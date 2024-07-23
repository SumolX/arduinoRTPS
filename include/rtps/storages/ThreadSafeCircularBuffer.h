/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include <array>
#include <limits>

#include "lwip/sys.h"
#include "semphr.h"

namespace rtps {

template <typename T, uint16_t SIZE> class ThreadSafeCircularBuffer {

public:
  bool init();

  bool moveElementIntoBuffer(T &&elem);
  bool copyElementIntoBuffer(const T &elem);

  /**
   * Removes the first into the given hull. Also moves responsibility for
   * resources.
   * @return true if element was injected. False if no element was present.
   */
  bool moveFirstInto(T &hull);
  bool peakFirst(T &hull);

  uint32_t numElements();
  uint32_t insertionFailures();

  void clear();

private:
  std::array<T, SIZE + 1> m_buffer{};
  uint16_t m_head = 0;
  uint16_t m_tail = 0;
  uint32_t m_num_elements = 0;
  uint32_t m_insertion_failures = 0;
  static_assert(SIZE + 1 < std::numeric_limits<decltype(m_head)>::max(),
                "Iterator is large enough for given size");

  SemaphoreHandle_t m_mutex;
  bool m_initialized = false;

  inline bool isFull();
  inline void incrementIterator(uint16_t &iterator);
  inline void incrementTail();
  inline void incrementHead();
};

}

#include "ThreadSafeCircularBuffer.tpp"
