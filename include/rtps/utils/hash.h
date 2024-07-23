/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

namespace rtps {

inline size_t hashCharArray(const char *p, size_t s) {
  size_t result = 0;
  const size_t prime = 31;
  for (size_t i = 0; i < s; ++i) {
    result = p[i] + (result * prime);
  }
  return result;
}

}
