/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "TcpipCoreLock.h"
#include "lwip/udp.h"

namespace rtps {

struct UdpConnection {
  udp_pcb *pcb = nullptr;
  uint16_t port = 0;

  UdpConnection() = default; // Required for static allocation

  explicit UdpConnection(uint16_t port) : port(port) {
    LOCK_TCPIP_CORE();
    pcb = udp_new();
    UNLOCK_TCPIP_CORE();
  }

  UdpConnection &operator=(UdpConnection &&other) noexcept {
    port = other.port;

    if (pcb != nullptr) {
      LOCK_TCPIP_CORE();
      udp_remove(pcb);
      UNLOCK_TCPIP_CORE();
    }
    pcb = other.pcb;
    other.pcb = nullptr;
    return *this;
  }

  ~UdpConnection() {
    if (pcb != nullptr) {
      LOCK_TCPIP_CORE();
      udp_remove(pcb);
      UNLOCK_TCPIP_CORE();
      pcb = nullptr;
    }
  }
};

}
