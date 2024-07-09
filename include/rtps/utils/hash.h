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
