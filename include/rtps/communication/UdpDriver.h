/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "UdpConnection.h"
#include "lwip/udp.h"
#include "rtps/common/types.h"
#include "rtps/communication/PacketInfo.h"
#include "rtps/config.h"
#include "rtps/storages/PBufWrapper.h"

#include <array>

namespace rtps {

class UdpDriver {

public:
  typedef void (*udpRxFunc_fp)(void *arg, udp_pcb *pcb, pbuf *p,
                               const ip_addr_t *addr, Ip4Port_t port);

  UdpDriver(udpRxFunc_fp callback, void *args);

  const rtps::UdpConnection *createUdpConnection(Ip4Port_t receivePort);
  bool joinMultiCastGroup(const IPAddress& addr) const;
  void sendPacket(PacketInfo &info);

  static bool isSameSubnet(const IPAddress& addr);
  static bool isMulticastAddress(const IPAddress& addr);

private:
  std::array<UdpConnection, Config::MAX_NUM_UDP_CONNECTIONS> m_conns;
  std::size_t m_numConns = 0;
  udpRxFunc_fp m_rxCallback = nullptr;
  void *m_callbackArgs = nullptr;

  bool sendPacket(const UdpConnection &conn, const IPAddress &destAddr,
                  Ip4Port_t destPort, pbuf &buffer);
};

}
