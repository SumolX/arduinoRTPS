/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include <lwip/tcpip.h>

namespace rtps {
class TcpipCoreLock {
public:
  TcpipCoreLock() { LOCK_TCPIP_CORE(); }
  ~TcpipCoreLock() { UNLOCK_TCPIP_CORE(); }
};

}
