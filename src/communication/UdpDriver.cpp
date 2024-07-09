#include "rtps/communication/UdpDriver.h"

#include "rtps/communication/TcpipCoreLock.h"
#include "rtps/utils/Lock.h"
#include "rtps/utils/Log.h"

#include <lwip/igmp.h>
#include <lwip/tcpip.h>
#include <string.h>

using rtps::UdpDriver;

#if UDP_DRIVER_VERBOSE && RTPS_GLOBAL_VERBOSE
#ifndef UDP_DRIVER_LOG
#include "rtps/utils/strutils.h"
#define UDP_DRIVER_LOG(...)                                                    \
  if (true) {                                                                  \
    printf("[UDP Driver] ");                                                   \
    printf(__VA_ARGS__);                                                       \
    printf("\r\n");                                                            \
  }
#endif
#else
#define UDP_DRIVER_LOG(...) //
#endif

UdpDriver::UdpDriver(rtps::UdpDriver::udpRxFunc_fp callback, void *args)
    : m_rxCallback(callback), m_callbackArgs(args) {}

const rtps::UdpConnection *
UdpDriver::createUdpConnection(Ip4Port_t receivePort) {
  for (uint8_t i = 0; i < m_numConns; ++i) {
    if (m_conns[i].port == receivePort) {
      return &m_conns[i];
    }
  }

  if (m_numConns == m_conns.size()) {
    return nullptr;
  }

  UdpConnection udp_conn(receivePort);

  {
    TcpipCoreLock lock;
    err_t err = udp_bind(udp_conn.pcb, IP_ADDR_ANY,
                         receivePort); // to receive multicast

    if (err != ERR_OK && err != ERR_USE) {
      return nullptr;
    }

    udp_recv(udp_conn.pcb, m_rxCallback, m_callbackArgs);
  }

  m_conns[m_numConns] = std::move(udp_conn);
  m_numConns++;

  UDP_DRIVER_LOG("Successfully created UDP connection on port %u \n",
                 receivePort);

  return &m_conns[m_numConns - 1];
}

bool UdpDriver::isSameSubnet(ip4_addr_t addr) {
#if PLATFORM_ESP32
  return (ip4_addr_netcmp(&addr, &(netif_default->ip_addr.u_addr.ip4),
                          &(netif_default->netmask.u_addr.ip4)) != 0);
#else
  return (ip4_addr_netcmp(&addr, &(netif_default->ip_addr),
                          &(netif_default->netmask)) != 0);
#endif
}

bool UdpDriver::isMulticastAddress(ip4_addr_t addr) {
#if IS_LITTLE_ENDIAN
  return (addr.addr & 0xF0) == 0xE0;
#else
  return (addr.addr >> 28) == 14;
#endif
}

bool UdpDriver::joinMultiCastGroup(ip4_addr_t addr) const {
  err_t iret;

  {
    TcpipCoreLock lock;
#if PLATFORM_ESP32
    iret = igmp_joingroup(ip_2_ip4(IP_ADDR_ANY), (&addr));
#else
    iret = igmp_joingroup(IP_ADDR_ANY, (&addr));
#endif
  }

  if (iret != ERR_OK) {
#if PLATFORM_ESP32
    ip_addr_t tmpAddr;
    memcpy((char *)&tmpAddr.u_addr.ip4, (char *)&addr.addr, sizeof(ip4_addr));
    tmpAddr.type = IPADDR_TYPE_V4;
    UDP_DRIVER_LOG("Failed to join IGMP multicast group %s\n",
                   ipaddr_ntoa(&tmpAddr));
#else
    UDP_DRIVER_LOG("Failed to join IGMP multicast group %s\n",
                   ipaddr_ntoa(&addr));
#endif

    return false;
  } else {
#if PLATFORM_ESP32
    ip_addr_t tmpAddr;
    memcpy((char *)&tmpAddr.u_addr.ip4, (char *)&addr.addr, sizeof(ip4_addr));
    tmpAddr.type = IPADDR_TYPE_V4;
    UDP_DRIVER_LOG("Succesfully joined  IGMP multicast group %s\n",
                   ipaddr_ntoa(&tmpAddr));
#else
    UDP_DRIVER_LOG("Succesfully joined  IGMP multicast group %s\n",
                   ipaddr_ntoa(&addr));
#endif
  }
  return true;
}

bool UdpDriver::sendPacket(const UdpConnection &conn, ip4_addr_t &destAddr,
                           Ip4Port_t destPort, pbuf &buffer) {
  err_t err;
  {
    TcpipCoreLock lock;
#if PLATFORM_ESP32
    ip_addr_t tmpAddr;
    memcpy((char *)&tmpAddr.u_addr.ip4, (char *)&destAddr.addr, sizeof(ip4_addr));
    tmpAddr.type = IPADDR_TYPE_V4;
    err = udp_sendto(conn.pcb, &buffer, &tmpAddr, destPort);
#else
    err = udp_sendto(conn.pcb, &buffer, &destAddr, destPort);
#endif
  }

  if (err != ERR_OK) {
#if PLATFORM_ESP32
    ip_addr_t tmpAddr;
    memcpy((char *)&tmpAddr.u_addr.ip4, (char *)&destAddr.addr, sizeof(ip4_addr));
    tmpAddr.type = IPADDR_TYPE_V4;
    UDP_DRIVER_LOG("UDP TRANSMIT NOT SUCCESSFUL %s:%u size: %u err: %i\n",
                   ipaddr_ntoa(&tmpAddr), destPort, buffer.tot_len, err);
#else
    UDP_DRIVER_LOG("UDP TRANSMIT NOT SUCCESSFUL %s:%u size: %u err: %i\n",
                   ipaddr_ntoa(&destAddr), destPort, buffer.tot_len, err);
#endif
    return false;
  }
  return true;
}

void UdpDriver::sendPacket(PacketInfo &packet) {
  auto p_conn = createUdpConnection(packet.srcPort);
  if (p_conn == nullptr) {
    ;

    UDP_DRIVER_LOG("Failed to create connection on port %u \n", packet.srcPort);

    return;
  }

  sendPacket(*p_conn, packet.destAddr, packet.destPort,
             *packet.buffer.firstElement);
}
