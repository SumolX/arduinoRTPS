#include "rtps/config.h"

namespace rtps {

IPAddress Config::IP_ADDRESS(192, 168, 4, 2);
GuidPrefix_t Config::BASE_GUID_PREFIX = GUID_RANDOM;
uint8_t Config::DOMAIN_ID = 0;

};
