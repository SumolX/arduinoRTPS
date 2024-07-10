#pragma once

#include <array>

#include "rtps/communication/UdpDriver.h"
#include "rtps/utils/udpUtils.h"
#include "ucdr/microcdr.h"

namespace rtps {
enum class LocatorKind_t : int32_t {
  LOCATOR_KIND_INVALID = -1,
  LOCATOR_KIND_RESERVED = 0,
  LOCATOR_KIND_UDPv4 = 1,
  LOCATOR_KIND_UDPv6 = 2
};

const uint32_t LOCATOR_PORT_INVALID = 0;
const std::array<uint8_t, 16> LOCATOR_ADDRESS_INVALID = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
 * This representation corresponds to the RTPS wire format
 */
struct FullLengthLocator {
  LocatorKind_t kind = LocatorKind_t::LOCATOR_KIND_INVALID;
  uint32_t port = LOCATOR_PORT_INVALID;
  std::array<uint8_t, 16> address =
      LOCATOR_ADDRESS_INVALID; // TODO make private such that kind and address
                               // always match?

  static FullLengthLocator createUDPv4Locator(const IPAddress& ip, uint32_t port) {
    FullLengthLocator locator;
    locator.kind = LocatorKind_t::LOCATOR_KIND_UDPv4;
    locator.port = port;
    locator.address =
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ip[0], ip[1], ip[2], ip[3]};

    return locator;
  }

  void setInvalid() { kind = LocatorKind_t::LOCATOR_KIND_INVALID; }

  bool isValid() const { return kind != LocatorKind_t::LOCATOR_KIND_INVALID; }

  bool readFromUcdrBuffer(ucdrBuffer &buffer) {
    if (ucdr_buffer_remaining(&buffer) < sizeof(FullLengthLocator)) {
      return false;
    } else {
      ucdr_deserialize_array_uint8_t(&buffer, reinterpret_cast<uint8_t *>(this),
                                     sizeof(FullLengthLocator));
      return true;
    }
  }

  bool serializeIntoUdcrBuffer(ucdrBuffer &buffer) {
    if (ucdr_buffer_remaining(&buffer) < sizeof(FullLengthLocator)) {
      return false;
    } else {
      ucdr_serialize_array_uint8_t(&buffer, reinterpret_cast<uint8_t *>(this),
                                   sizeof(FullLengthLocator));
    }
  }

  IPAddress getIp4Address() const {
    return IPAddress(address[12], address[13], address[14], address[15]);
  }

  bool isSameAddress(const IPAddress& address) {
    return getIp4Address() == address;
  }

  inline bool isSameSubnet() const {
    return UdpDriver::isSameSubnet(getIp4Address());
  }

  inline bool isMulticastAddress() const {
    return UdpDriver::isMulticastAddress(getIp4Address());
  }

  inline uint32_t getLocatorPort() { return static_cast<Ip4Port_t>(port); }

} __attribute__((packed));

inline FullLengthLocator getBuiltInUnicastLocator(ParticipantId_t participantId) {
  return FullLengthLocator::createUDPv4Locator(Config::ADDRESS, getBuiltInUnicastPort(participantId));
}

inline FullLengthLocator getUserUnicastLocator(ParticipantId_t participantId) {
  return FullLengthLocator::createUDPv4Locator(Config::ADDRESS, getUserUnicastPort(participantId));
}

inline FullLengthLocator getUserMulticastLocator() {
  return FullLengthLocator::createUDPv4Locator(Config::ADDRESS, getUserMulticastPort());
}

inline FullLengthLocator getBuiltInMulticastLocator() {
  return FullLengthLocator::createUDPv4Locator(IPAddress(239, 255, 0, 1), getBuiltInMulticastPort());
}

inline FullLengthLocator getDefaultSendMulticastLocator() {
  return FullLengthLocator::createUDPv4Locator(IPAddress(239, 255, 0, 1), getBuiltInMulticastPort());
}

/*
 * This representation omits unnecessary 12 bytes of the full RTPS wire format
 */
struct LocatorIPv4 {
  LocatorKind_t kind = LocatorKind_t::LOCATOR_KIND_INVALID;
  std::array<uint8_t, 4> address = {0};
  uint32_t port = LOCATOR_PORT_INVALID;

  LocatorIPv4() = default;
  LocatorIPv4(const FullLengthLocator &locator) {
    address[0] = locator.address[12];
    address[1] = locator.address[13];
    address[2] = locator.address[14];
    address[3] = locator.address[15];
    port = locator.port;
    kind = locator.kind;
  }

  IPAddress getIp4Address() const {
    return IPAddress(address[0], address[1], address[2], address[3]);
  }

  void setInvalid() { kind = LocatorKind_t::LOCATOR_KIND_INVALID; }

  bool isValid() const { return kind != LocatorKind_t::LOCATOR_KIND_INVALID; }

  inline bool isSameSubnet() const {
    return UdpDriver::isSameSubnet(getIp4Address());
  }

  inline bool isMulticastAddress() const {
    return UdpDriver::isMulticastAddress(getIp4Address());
  }
};

}
