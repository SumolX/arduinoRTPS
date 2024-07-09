#pragma once

#include <lwip/tcpip.h>

namespace rtps {
class TcpipCoreLock {
public:
  TcpipCoreLock() { LOCK_TCPIP_CORE(); }
  ~TcpipCoreLock() { UNLOCK_TCPIP_CORE(); }
};

}
