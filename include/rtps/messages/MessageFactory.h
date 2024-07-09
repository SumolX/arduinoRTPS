#pragma once

#include <array>
#include <cstdint>

#include "rtps/common/types.h"
#include "rtps/config.h"
#include "rtps/messages/MessageTypes.h"
#include "rtps/utils/sysFunctions.h"

namespace rtps {
namespace MessageFactory {
const std::array<uint8_t, 4> PROTOCOL_TYPE{'R', 'T', 'P', 'S'};
const uint8_t numBytesUntilEndOfLength =
    4; // The first bytes incl. submessagelength don't count

template <class Buffer>
void addHeader(Buffer &buffer, const GuidPrefix_t &guidPrefix) {

  Header header;
  header.protocolName = PROTOCOL_TYPE;
  header.protocolVersion = PROTOCOLVERSION;
  header.vendorId = Config::VENDOR_ID;
  header.guidPrefix = guidPrefix;

  serializeMessage(buffer, header);
}

template <class Buffer>
bool addSubMessageInfoDST(Buffer &buffer, GuidPrefix_t &dst) {
  SubmessageInfoDST msg;
  msg.header.submessageId = SubmessageKind::INFO_DST;

#if IS_LITTLE_ENDIAN
  msg.header.flags = FLAG_LITTLE_ENDIAN;
#else
  msg.header.flags = FLAG_BIG_ENDIAN;
#endif

  msg.header.octetsToNextHeader = sizeof(GuidPrefix_t);
  msg.guidPrefix = dst;

  return serializeMessage(buffer, msg);
}

template <class Buffer>
void addSubMessageTimeStamp(Buffer &buffer, bool setInvalid = false) {
  SubmessageHeader header;
  header.submessageId = SubmessageKind::INFO_TS;

#if IS_LITTLE_ENDIAN
  header.flags = FLAG_LITTLE_ENDIAN;
#else
  header.flags = FLAG_BIG_ENDIAN;
#endif

  if (setInvalid) {
    header.flags |= FLAG_INVALIDATE;
    header.octetsToNextHeader = 0;
  } else {
    header.octetsToNextHeader = sizeof(Time_t);
  }

  serializeMessage(buffer, header);

  if (!setInvalid) {
    buffer.reserve(header.octetsToNextHeader);
    Time_t now = getCurrentTimeStamp();
    buffer.append(reinterpret_cast<uint8_t *>(&now.seconds),
                  sizeof(Time_t::seconds));
    buffer.append(reinterpret_cast<uint8_t *>(&now.fraction),
                  sizeof(Time_t::fraction));
  }
}

template <class Buffer>
void addSubMessageData(Buffer &buffer, const Buffer &filledPayload,
                       bool containsInlineQos, const SequenceNumber_t &SN,
                       const EntityId_t &writerID, const EntityId_t &readerID) {
  SubmessageData msg;
  msg.header.submessageId = SubmessageKind::DATA;
#if IS_LITTLE_ENDIAN
  msg.header.flags = FLAG_LITTLE_ENDIAN;
#else
  msg.header.flags = FLAG_BIG_ENDIAN;
#endif

  msg.header.octetsToNextHeader = SubmessageData::getRawSize() +
                                  filledPayload.spaceUsed() -
                                  numBytesUntilEndOfLength;

  if (containsInlineQos) {
    msg.header.flags |= FLAG_INLINE_QOS;
  }
  if (filledPayload.isValid()) {
    msg.header.flags |= FLAG_DATA_PAYLOAD;
  }

  msg.writerSN = SN;
  msg.extraFlags = 0;
  msg.readerId = readerID;
  msg.writerId = writerID;

  constexpr uint16_t octetsToInlineQoS =
      4 + 4 + 8; // EntityIds + SequenceNumber
  msg.octetsToInlineQos = octetsToInlineQoS;

  serializeMessage(buffer, msg);

  if (filledPayload.isValid()) {
    buffer.append(filledPayload);
  }
}

template <class Buffer>
void addHeartbeat(Buffer &buffer, EntityId_t writerId, EntityId_t readerId,
                  SequenceNumber_t firstSN, SequenceNumber_t lastSN,
                  Count_t count) {
  SubmessageHeartbeat subMsg;
  subMsg.header.submessageId = SubmessageKind::HEARTBEAT;
  subMsg.header.octetsToNextHeader =
      SubmessageHeartbeat::getRawSize() - numBytesUntilEndOfLength;
#if IS_LITTLE_ENDIAN
  subMsg.header.flags = FLAG_LITTLE_ENDIAN;
#else
  subMsg.header.flags = FLAG_BIG_ENDIAN;
#endif
  // Force response by not setting final flag.

  subMsg.writerId = writerId;
  subMsg.readerId = readerId;
  subMsg.firstSN = firstSN;
  subMsg.lastSN = lastSN;
  subMsg.count = count;

  serializeMessage(buffer, subMsg);
}

template <class Buffer>
void addAckNack(Buffer &buffer, EntityId_t writerId, EntityId_t readerId,
                SequenceNumberSet readerSNState, Count_t count,
                bool final_flag) {
  SubmessageAckNack subMsg;
  subMsg.header.submessageId = SubmessageKind::ACKNACK;
#if IS_LITTLE_ENDIAN
  subMsg.header.flags = FLAG_LITTLE_ENDIAN;
#else
  subMsg.header.flags = FLAG_BIG_ENDIAN;
#endif
  if (final_flag) {
    subMsg.header.flags |= FLAG_FINAL; // For now, we don't want any response
  } else {
    subMsg.header.flags &=
        ~FLAG_FINAL; // Send future heartbeats, even if no change occured
  }
  subMsg.header.octetsToNextHeader =
      SubmessageAckNack::getRawSize(readerSNState) - numBytesUntilEndOfLength;

  subMsg.writerId = writerId;
  subMsg.readerId = readerId;
  subMsg.readerSNState = readerSNState;
  subMsg.count = count;

  serializeMessage(buffer, subMsg);
}

template <class Buffer>
void addSubmessageGap(Buffer &buffer, EntityId_t writerId, EntityId_t readerId,
                      const SequenceNumber_t &firstMissing,
                      const SequenceNumber_t &nextValid) {
  SubmessageGap subMsg;
  subMsg.header.submessageId = SubmessageKind::GAP;
#if IS_LITTLE_ENDIAN
  subMsg.header.flags = FLAG_LITTLE_ENDIAN;
#else
  subMsg.header.flags = FLAG_BIG_ENDIAN;
#endif
  subMsg.header.octetsToNextHeader = 32;

  subMsg.writerId = writerId;
  subMsg.readerId = readerId;
  subMsg.gapStart = firstMissing;
  subMsg.gapList.base = nextValid;
  subMsg.gapList.numBits = 0;

  serializeMessage(buffer, subMsg);
}

}

}
