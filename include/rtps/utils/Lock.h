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
