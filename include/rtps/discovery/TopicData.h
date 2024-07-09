#pragma once

#define SUPPRESS_UNICAST 0

#include "rtps/config.h"
#include "rtps/utils/hash.h"
#include "ucdr/microcdr.h"
#include <array>
#include <rtps/common/Locator.h>

namespace rtps {

struct BuiltInTopicKey {
  std::array<uint32_t, 3> value;
};

struct TopicData {
  Guid_t endpointGuid;
  char typeName[Config::MAX_TYPENAME_LENGTH];
  char topicName[Config::MAX_TOPICNAME_LENGTH];
  ReliabilityKind_t reliabilityKind;
  DurabilityKind_t durabilityKind;
  FullLengthLocator unicastLocator;
  FullLengthLocator multicastLocator;

  uint8_t statusInfo;
  bool statusInfoValid;
  // Use Case: Remotes communicates id of deleted endpoint through key_hash
  // parameter
  EntityId_t entityIdFromKeyHash;
  bool entityIdFromKeyHashValid;

  TopicData()
      : endpointGuid(GUID_UNKNOWN), typeName{'\0'}, topicName{'\0'},
        reliabilityKind(ReliabilityKind_t::BEST_EFFORT),
        durabilityKind(DurabilityKind_t::VOLATILE) {
    rtps::FullLengthLocator someLocator =
        rtps::FullLengthLocator::createUDPv4Locator(
            192, 168, 0, 42, rtps::getUserUnicastPort(0));
    unicastLocator = someLocator;
    multicastLocator = FullLengthLocator();
  };

  TopicData(Guid_t guid, ReliabilityKind_t reliability, FullLengthLocator loc)
      : endpointGuid(guid), typeName{'\0'}, topicName{'\0'},
        reliabilityKind(reliability),
        durabilityKind(DurabilityKind_t::VOLATILE), unicastLocator(loc) {}


  bool matchesTopicOf(const TopicData &other);

  bool readFromUcdrBuffer(ucdrBuffer &buffer);
  bool serializeIntoUcdrBuffer(ucdrBuffer &buffer) const;

  bool isDisposedFlagSet() const;
  bool isUnregisteredFlagSet() const;
};

struct TopicDataCompressed {
  Guid_t endpointGuid;
  std::size_t topicHash;
  std::size_t typeHash;
  bool is_reliable;
  LocatorIPv4 unicastLocator;
  LocatorIPv4 multicastLocator;

  TopicDataCompressed() = default;
  TopicDataCompressed(const TopicData &topic_data) {
    endpointGuid = topic_data.endpointGuid;
    topicHash =
        hashCharArray(topic_data.topicName, Config::MAX_TOPICNAME_LENGTH);
    typeHash = hashCharArray(topic_data.typeName, Config::MAX_TYPENAME_LENGTH);
    is_reliable = (topic_data.reliabilityKind == ReliabilityKind_t::RELIABLE)
                      ? true
                      : false;
    unicastLocator = topic_data.unicastLocator;
    multicastLocator = topic_data.multicastLocator;
  }

  bool matchesTopicOf(const TopicData &topic_data) const;
};

}
