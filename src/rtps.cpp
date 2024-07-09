#include "rtps/rtps.h"

#include "IPAddress.h"
#include "Stream.h"

namespace rtps {

RTPS::RTPS(const IPAddress& ipAddress, Stream& stream)
  : _ipAddress(ipAddress)
  , _stream(stream)
  , _written(0)
  , _buffer{0} {
}

void RTPS::begin() {
}

void RTPS::update() {
  size_t processed = 0;
  size_t available = _stream.available();
  if (available) {
    for (size_t i = 0; i < available; ++i) {
      _buffer[_written++] = _stream.read();
      processed = onReadHandler(_buffer, _written);
      if (processed > 0) {
        memcpy(_buffer, &_buffer[processed], _written - processed);
        _written -= processed;
      }
    }
  }
}

void RTPS::write(const uint8_t* buffer, size_t size) {
  _stream.write(buffer, size);
}

};
