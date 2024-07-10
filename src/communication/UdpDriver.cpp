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

bool UdpDriver::isSameSubnet(const IPAddress& addr) {
  return ((uint32_t)Config::NETWORK & (uint32_t)addr) ==
         ((uint32_t)Config::NETWORK & (uint32_t)Config::ADDRESS);
}

bool UdpDriver::isMulticastAddress(const IPAddress& addr) {
#if IS_LITTLE_ENDIAN
  return ((uint32_t)addr & 0xF0) == 0xE0;
#else
  return ((uint32_t)addr >> 28) == 14;
#endif
}

bool UdpDriver::joinMultiCastGroup(const IPAddress& addr) const {
  err_t iret;

  {
    TcpipCoreLock lock;
    ip4_addr_t groupaddr = { .addr = (uint32_t)addr };
    iret = igmp_joingroup(IP4_ADDR_ANY4, &groupaddr);
  }

  if (iret != ERR_OK) {
    UDP_DRIVER_LOG("Failed to join IGMP multicast group %s\n", addr.toString().c_str());
    return false;
  } else {
    UDP_DRIVER_LOG("Succesfully joined  IGMP multicast group %s\n", addr.toString().c_str());
  }
  return true;
}

bool UdpDriver::sendPacket(const UdpConnection &conn, const IPAddress &destAddr,
                           Ip4Port_t destPort, pbuf &buffer) {
  err_t err;
  {
    TcpipCoreLock lock;
    ip_addr_t dst = IPADDR4_INIT((uint32_t)destAddr);
    err = udp_sendto(conn.pcb, &buffer, &dst, destPort);
  }

  if (err != ERR_OK) {
    UDP_DRIVER_LOG("UDP TRANSMIT NOT SUCCESSFUL %s:%u size: %u err: %i\n",
                   destAddr.toString().c_str(), destPort, buffer.tot_len, err);
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
