/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

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
