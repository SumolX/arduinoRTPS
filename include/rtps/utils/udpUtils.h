#pragma once

#include "rtps/config.h"

namespace rtps {
namespace {
const uint16_t PB = 7400; // Port Base Number
const uint16_t DG = 250;  // DomainId Gain
const uint16_t PG = 2;    // ParticipantId Gain
// Additional Offsets
const uint16_t D0 = 0;  // Builtin multicast
const uint16_t D1 = 10; // Builtin unicast
const uint16_t D2 = 1;  // User multicast
const uint16_t D3 = 11; // User unicast
} // namespace

constexpr ip4_addr transformIP4ToU32(uint8_t MSB, uint8_t p2, uint8_t p1,
                                     uint8_t LSB) {
  return ip4_addr{PP_HTONL(LWIP_MAKEU32(MSB, p2, p1, LSB))};
}

inline Ip4Port_t getBuiltInUnicastPort(ParticipantId_t participantId) {
  return PB + DG * Config::DOMAIN_ID + D1 + PG * participantId;
}

inline Ip4Port_t getBuiltInMulticastPort() {
  return PB + DG * Config::DOMAIN_ID + D0;
}

inline Ip4Port_t getUserUnicastPort(ParticipantId_t participantId) {
  return PB + DG * Config::DOMAIN_ID + D3 + PG * participantId;
}

inline Ip4Port_t getUserMulticastPort() {
  return PB + DG * Config::DOMAIN_ID + D2;
}

constexpr bool isUserPort(Ip4Port_t port) {
  return (port & 1) == 1;
} // really useful? There may be other user ports than just odd ones?

inline bool isMultiCastPort(Ip4Port_t port) {
  const auto idWithoutBase = port - PB - DG * Config::DOMAIN_ID;
  return idWithoutBase == D0 ||
         (idWithoutBase >= D2 &&
          idWithoutBase < D3); // There are several UserMulticastPorts!
}

inline bool isMetaMultiCastPort(Ip4Port_t port) {
  const auto idWithoutBase = port - PB - DG * Config::DOMAIN_ID;
  return idWithoutBase == D0;
}

inline bool isUserMultiCastPort(Ip4Port_t port) {
  const auto idWithoutBase = port - PB - DG * Config::DOMAIN_ID;
  return (idWithoutBase >= D2 && idWithoutBase < D1);
}

inline bool isZeroAddress(ip4_addr_t address) { return address.addr == 0; }

inline ParticipantId_t getParticipantIdFromUnicastPort(Ip4Port_t port,
                                                       bool isUserPort) {

  const auto basePart = PB + DG * Config::DOMAIN_ID;
  ParticipantId_t participantPart = port - basePart;

  uint16_t offset = 0;
  if (isUserPort) {
    offset = D3;
  } else {
    offset = D1;
  }

  participantPart -= offset;

  auto id = static_cast<ParticipantId_t>(participantPart / PG);
  if (id * PG + basePart + offset == port) {
    return id;
  } else {
    return PARTICIPANT_ID_INVALID;
  }
}

}
