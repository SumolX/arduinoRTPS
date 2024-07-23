/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "rtps/config.h"
#include "rtps/storages/CacheChange.h"

namespace rtps {

/**
 * Simple version of a history cache. It sets consecutive sequence numbers
 * automatically which allows an easy and fast approach of dropping acknowledged
 * changes. Furthermore, disposing of arbitrary changes is not possible.
 * However, this is in principle easy to add by changing the ChangeKind and
 * dropping it when passing it during deleting of other sequence numbers
 */
template <uint16_t SIZE> class SimpleHistoryCache {
public:
  SimpleHistoryCache() = default;

  bool isFull() const {
    uint16_t it = m_head;
    incrementIterator(it);
    return it == m_tail;
  }

  const CacheChange *addChange(const uint8_t *data, DataSize_t size,
                               bool inLineQoS, bool disposeAfterWrite) {
    CacheChange change;
    change.kind = ChangeKind_t::ALIVE;
    change.inLineQoS = inLineQoS;
    change.disposeAfterWrite = disposeAfterWrite;
    change.data.reserve(size);
    change.data.append(data, size);
    change.sequenceNumber = ++m_lastUsedSequenceNumber;

    CacheChange *place = &m_buffer[m_head];
    incrementHead();

    *place = std::move(change);
    return place;
  }

  const CacheChange *addChange(const uint8_t *data, DataSize_t size) {
    return addChange(data, size, 0, false);
  }

  void removeUntilIncl(SequenceNumber_t sn) {
    if (m_head == m_tail) {
      return;
    }

    if (getSeqNumMax() <= sn) { // We won't overrun head
      m_head = m_tail;
      return;
    }

    while (m_buffer[m_tail].sequenceNumber <= sn && (m_head != m_tail)) {
      incrementTail();
    }
  }

  void dropOldest() { removeUntilIncl(getSeqNumMin()); }

  bool setCacheChangeKind(const SequenceNumber_t &sn, ChangeKind_t kind) {
    CacheChange *change = getChangeBySN(sn);
    if (change == nullptr) {
      return false;
    }

    change->kind = kind;
    return true;
  }

  CacheChange *getChangeBySN(SequenceNumber_t sn) {
    SequenceNumber_t minSN = getSeqNumMin();
    if (sn < minSN || getSeqNumMax() < sn) {
      return nullptr;
    }
    static_assert(std::is_unsigned<decltype(sn.low)>::value,
                  "Underflow well defined");
    static_assert(sizeof(m_tail) <= sizeof(uint16_t), "Cast ist well defined");
    // We don't overtake head, therefore difference of sn is within same range
    // as iterators
    uint16_t pos = m_tail + static_cast<uint16_t>(sn.low - minSN.low);

    // Diff is smaller than the size of the array -> max one overflow
    if (pos >= m_buffer.size()) {
      pos -= m_buffer.size();
    }
    return &m_buffer[pos];
  }

  const SequenceNumber_t &getSeqNumMin() const {
    if (m_head == m_tail) {
      return SEQUENCENUMBER_UNKNOWN;
    } else {
      return m_buffer[m_tail].sequenceNumber;
    }
  }

  const SequenceNumber_t &getSeqNumMax() const {
    if (m_head == m_tail) {
      return SEQUENCENUMBER_UNKNOWN;
    } else {
      return m_lastUsedSequenceNumber;
    }
  }

  void clear() {
    m_head = 0;
    m_tail = 0;
    m_lastUsedSequenceNumber = {0, 0};
  }

private:
  std::array<CacheChange, SIZE + 1> m_buffer{};
  uint16_t m_head = 0;
  uint16_t m_tail = 0;
  static_assert(sizeof(SIZE) <= sizeof(m_head),
                "Iterator is large enough for given size");

  SequenceNumber_t m_lastUsedSequenceNumber{0, 0};

  inline void incrementHead() {
    incrementIterator(m_head);
    if (m_head == m_tail) {
      // Move without check
      incrementIterator(m_tail); // drop one
    }
  }

  inline void incrementIterator(uint16_t &iterator) const {
    ++iterator;
    if (iterator >= m_buffer.size()) {
      iterator = 0;
    }
  }

  inline void incrementTail() {
    if (m_head != m_tail) {
      incrementIterator(m_tail);
    }
  }

protected:
  // This constructor was created for unit testing
  explicit SimpleHistoryCache(SequenceNumber_t lastUsed)
      : SimpleHistoryCache() {
    m_lastUsedSequenceNumber = lastUsed;
  }
};

}
