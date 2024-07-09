#pragma once

#include <functional>

#include <Arduino.h>

#include "rtps/entities/Domain.h"

/**
 *  Forward Declarations
 */
class IPAddress;
class Stream;

namespace rtps {

struct Comms {
  /**
   *  Destructor
   */
  virtual ~Comms() = default;

  /**
   *  Methods
   */
  virtual void write(const uint8_t* data, size_t size) = 0;

  /**
   *  Members
   */
  std::function<size_t(const uint8_t* data, size_t size)> onReadHandler;
};

class RTPS : public Comms {
public:

    /**
     *  Destructor/Constructor
     */
    ~RTPS() = default;
    RTPS(const IPAddress& ipAddress, Stream& stream);

    /**
     *  Initialization routine.
     */
    void begin();

    /**
     *  Non-blocking data acquisition routine.
     */
    void update();

    /**
     *  Provide external access to write routine.
     */
    void write(const uint8_t* buffer, size_t size);

    /**
     *  Accessors
     */
    const IPAddress& getIpAddress();

private:

    /**
     *  Default Constructor Disabled
     */
    RTPS() = delete;

    /**
     *  Members
     */
    const IPAddress& _ipAddress;
    Stream&          _stream;
    size_t           _written;
    uint8_t          _buffer[512];
    rtps::Domain     _domain;
};

};
