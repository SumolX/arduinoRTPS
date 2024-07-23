/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#include "rtps/utils/Lock.h"

namespace rtps {

bool createMutex(SemaphoreHandle_t *mutex) {
  *mutex = xSemaphoreCreateRecursiveMutex();
  if (*mutex != NULL) {
    return true;
  } else {
    LWIP_ASSERT("Mutex creation failed", true);
    return false;
  }
}

}
