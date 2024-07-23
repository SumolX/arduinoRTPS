/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include <cstdint>

#include "rtps/common/types.h"
#include "rtps/config.h"
#include "rtps/discovery/BuiltInEndpoints.h"

namespace rtps {
class Reader;
class Writer;
class Participant;
class MessageProcessingInfo;

class MessageReceiver {
public:
  GuidPrefix_t sourceGuidPrefix = GUIDPREFIX_UNKNOWN;
  ProtocolVersion_t sourceVersion = PROTOCOLVERSION;
  VendorId_t sourceVendor = VENDOR_UNKNOWN;
  bool haveTimeStamp = false;

  explicit MessageReceiver(Participant *part);

  bool processMessage(const uint8_t *data, DataSize_t size);

private:
  Participant *mp_part;

  void resetState();

  // TODO make msgInfo a member
  // This probably make processing faster, as no parameter needs to be passed
  // around However, we need to make sure data is set to nullptr after
  // processMsg to make sure we don't access it again afterwards.
  /**
   * Check header for validity, modifies the state of the receiver and
   * adjusts the position of msgInfo accordingly
   */
  bool processGapSubmessage(MessageProcessingInfo &msgInfo);
  bool processHeader(MessageProcessingInfo &msgInfo);
  bool processSubmessage(MessageProcessingInfo &msgInfo,
                         const SubmessageHeader &submsgHeader);
  bool processDataSubmessage(MessageProcessingInfo &msgInfo,
                             const SubmessageHeader &submsgHeader);
  bool processHeartbeatSubmessage(MessageProcessingInfo &msgInfo);
  bool processAckNackSubmessage(MessageProcessingInfo &msgInfo);
};

}
