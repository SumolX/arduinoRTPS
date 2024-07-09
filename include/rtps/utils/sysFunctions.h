#pragma once

#include "lwip/sys.h"
#include "rtps/common/types.h"

namespace rtps {

inline Time_t getCurrentTimeStamp() {
  Time_t now;
  // TODO FIX
  uint32_t nowMs = sys_now();
  now.seconds = (int32_t)nowMs / 1000;
  now.fraction = ((nowMs % 1000) / 1000);
  return now;
}

}
