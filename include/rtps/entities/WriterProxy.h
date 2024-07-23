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
#include <rtps/common/Locator.h>

namespace rtps {
struct WriterProxy {
  Guid_t remoteWriterGuid;
  SequenceNumber_t expectedSN;
  Count_t ackNackCount;
  Count_t hbCount;
  bool is_reliable;
  LocatorIPv4 remoteLocator;

  WriterProxy() = default;

  WriterProxy(const Guid_t &guid, const LocatorIPv4 &loc, bool reliable)
      : remoteWriterGuid(guid),
        expectedSN(SequenceNumber_t{0, 1}), ackNackCount{1}, hbCount{0},
        is_reliable(reliable), remoteLocator(loc) {}

  // For now, we don't store any packets, so we just request all starting from
  // the next expected
  SequenceNumberSet getMissing(const SequenceNumber_t &firstAvail,
                               const SequenceNumber_t &lastAvail) {
    SequenceNumberSet set;
    if (lastAvail < expectedSN) {
      set.base = expectedSN;
      set.numBits = 0;
    } else {
      set.base = expectedSN;
      SequenceNumber_t i;
      uint32_t bit;
      for (bit = 0, i = expectedSN; i <= lastAvail && bit < SNS_MAX_NUM_BITS;
           i++, bit++) {
        set.bitMap[0] |= uint32_t{1} << (31 - bit);
        set.numBits++;
      }
    }

    return set;
  }

  Count_t getNextAckNackCount() {
    const Count_t tmp = ackNackCount;
    ++ackNackCount.value;
    return tmp;
  }
};

}
