/**
 * Copyright © 2024 Paulo Cabral <sumolx@gmail.com>
 * 
 * This file is part of arduinoRTPS.
 * 
 * You should have received a copy of the MIT License along with arduinoRTPS.
 * If not, see <https://mit-license.org>.
 */

#include "rtps/config.h"

namespace rtps {
/**
 *  Default Network Configuration which
 *  can be overriden depending on interfaces.
 */
IPAddress Config::ADDRESS(192, 168, 4, 1);
IPAddress Config::NETMASK(255, 255, 255, 0);
GuidPrefix_t Config::BASE_GUID_PREFIX = GUID_RANDOM;
uint8_t Config::DOMAIN_ID = 0;

const IPAddress Config::MULTICAST(239, 255, 0, 1);
};
