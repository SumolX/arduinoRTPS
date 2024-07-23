/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "rtps/common/types.h"
#include "rtps/storages/PBufWrapper.h"

namespace rtps {
struct CacheChange {
  ChangeKind_t kind = ChangeKind_t::INVALID;
  bool inLineQoS = false;
  bool disposeAfterWrite = false;
  TickType_t sentTickCount = 0;
  SequenceNumber_t sequenceNumber = SEQUENCENUMBER_UNKNOWN;
  PBufWrapper data;

  CacheChange &operator=(const CacheChange &other) = delete;

  CacheChange &operator=(CacheChange &&other) noexcept {
	  kind = other.kind;
	  inLineQoS = other.inLineQoS;
	  disposeAfterWrite = other.disposeAfterWrite;
	  sentTickCount = other.sentTickCount;
	  sequenceNumber = other.sequenceNumber;
	  data = std::move(other.data);
	  return *this;
  }

  CacheChange() = default;
  CacheChange(ChangeKind_t kind, SequenceNumber_t sequenceNumber)
      : kind(kind), sequenceNumber(sequenceNumber){};

  void reset() {
    kind = ChangeKind_t::INVALID;
    sequenceNumber = SEQUENCENUMBER_UNKNOWN;
    inLineQoS = false;
    disposeAfterWrite = false;
    sentTickCount = 0;
  }

  bool isInitialized() { return (kind != ChangeKind_t::INVALID); }
};

}
