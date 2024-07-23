/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "FreeRTOS.h"
#include "lwip/sys.h"
#include "semphr.h"

namespace rtps {

class Lock {
public:
  explicit Lock(SemaphoreHandle_t &mutex) : m_mutex(mutex) {
    xSemaphoreTakeRecursive(m_mutex, portMAX_DELAY);
  };

  ~Lock() { xSemaphoreGiveRecursive(m_mutex); };

private:
  SemaphoreHandle_t m_mutex;
};

bool createMutex(SemaphoreHandle_t *mutex);

}
