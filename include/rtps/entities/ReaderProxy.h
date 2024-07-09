#pragma once

#include "rtps/common/types.h"
#include "rtps/discovery/ParticipantProxyData.h"

namespace rtps {
struct ReaderProxy {
  Guid_t remoteReaderGuid;
  Count_t ackNackCount = {0};
  LocatorIPv4 remoteLocator;
  bool is_reliable = false;
  LocatorIPv4 remoteMulticastLocator;
  bool useMulticast = false;
  bool suppressUnicast = false;
  bool unknown_eid = false;
  bool finalFlag = false;
  SequenceNumber_t lastAckNackSequenceNumber = {0, 1};

  ReaderProxy()
      : remoteReaderGuid({GUIDPREFIX_UNKNOWN, ENTITYID_UNKNOWN}),
        ackNackCount{0}, remoteLocator(LocatorIPv4()), finalFlag(false){};
  ReaderProxy(const Guid_t &guid, const LocatorIPv4 &loc, bool reliable)
      : remoteReaderGuid(guid), remoteLocator(loc),
        is_reliable(reliable), ackNackCount{0}, finalFlag(false){};
  ReaderProxy(const Guid_t &guid, const LocatorIPv4 &loc,
              const LocatorIPv4 &mcastloc, bool reliable)
      : remoteReaderGuid(guid), remoteLocator(loc), is_reliable(reliable),
        remoteMulticastLocator(mcastloc), ackNackCount{0}, finalFlag(false){};
};

}
