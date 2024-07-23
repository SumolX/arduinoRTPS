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

inline int entityId2Str(rtps::EntityId_t id, char* str, size_t size) {
  int bytes = 0;
  for (const auto byte : id.entityKey) {
    bytes += snprintf(&str[bytes], size - bytes, "%x", (int)byte);
  }
  bytes +=
    snprintf(
      &str[bytes],
      size - bytes,
      "%x",
      static_cast<uint8_t>(id.entityKind)
  );
  return bytes;
}

inline int guidPrefix2Str(rtps::GuidPrefix_t prefix, char* str, size_t size) {
  int bytes = 0;
  for (const auto byte : prefix.id) {
    bytes +=  snprintf(&str[bytes], size - bytes, "%x", (int)byte);
  }
  return bytes;
}

inline int guid2Str(rtps::Guid_t guid, char* str, size_t size) {
  int bytes = guidPrefix2Str(guid.prefix, str, size);
  bytes += snprintf(&str[bytes], size - bytes, ":");
  bytes += entityId2Str(guid.entityId, &str[bytes], size - bytes);
  return bytes;
}
