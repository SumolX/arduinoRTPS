#pragma once

#include "lwip/pbuf.h"
#include "rtps/common/types.h"

namespace rtps {

struct PBufWrapper {

  pbuf *firstElement = nullptr;

  PBufWrapper() = default;
  explicit PBufWrapper(pbuf *bufferToWrap);
  explicit PBufWrapper(DataSize_t length);

  PBufWrapper(const PBufWrapper &other) = delete;
  PBufWrapper &operator=(const PBufWrapper &other) = delete;

  PBufWrapper(PBufWrapper &&other) noexcept;
  PBufWrapper &operator=(PBufWrapper &&other) noexcept;

  ~PBufWrapper();

  bool isValid() const;

  bool append(const uint8_t *data, DataSize_t length);

  /// Note that unused reserved memory is now part of the wrapper. New calls to
  /// append(uint8_t*[...]) will continue behind the appended wrapper
  void append(const PBufWrapper &other);

  bool reserve(DataSize_t length);

  void destroy();

  /// After calling this function, data is added starting from the beginning
  /// again. It does not revert reserve.
  void reset();

  DataSize_t spaceLeft() const;
  DataSize_t spaceUsed() const;

private:
  constexpr static pbuf_layer m_layer = PBUF_TRANSPORT;
  constexpr static pbuf_type m_type = PBUF_POOL;

  DataSize_t m_freeSpace = 0;

  bool increaseSizeBy(uint16_t length);

  void copySimpleMembersAndResetBuffer(const PBufWrapper &other);
};

}
