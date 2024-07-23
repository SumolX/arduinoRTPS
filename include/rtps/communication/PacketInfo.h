/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include <IPAddress.h>

#include "rtps/common/types.h"
#include "rtps/storages/PBufWrapper.h"

namespace rtps {

struct PacketInfo {
  Ip4Port_t srcPort; // TODO Do we need that?
  IPAddress destAddr;
  Ip4Port_t destPort;
  PBufWrapper buffer;

  void copyTriviallyCopyable(const PacketInfo &other) {
    this->srcPort = other.srcPort;
    this->destPort = other.destPort;
    this->destAddr = other.destAddr;
  }

  PacketInfo() = default;
  ~PacketInfo() = default;

  PacketInfo &operator=(const PacketInfo &other) = delete;

  PacketInfo &operator=(PacketInfo &&other) noexcept {
    copyTriviallyCopyable(other);
    this->buffer = std::move(other.buffer);
    return *this;
  }
};

}
