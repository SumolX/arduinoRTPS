/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include <cstdio>
#include <stdarg.h>

#define RTPS_GLOBAL_VERBOSE 0

#define SFW_VERBOSE 0
#define SPDP_VERBOSE 0
#define PBUF_WRAP_VERBOSE 0
#define SEDP_VERBOSE 0
#define RECV_VERBOSE 0
#define PARTICIPANT_VERBOSE 0
#define DOMAIN_VERBOSE 0
#define UDP_DRIVER_VERBOSE 0
#define TSCB_VERBOSE 0
#define SLW_VERBOSE 0
#define SFR_VERBOSE 0
#define SLR_VERBOSE 0
#define THREAD_POOL_VERBOSE 0

#undef SFW_LOG
#undef SPDP_LOG
#undef PBUF_LOG
#undef SEDP_LOG
#undef RECV_LOG
#undef PARTICIPANT_LOG
#undef DOMAIN_LOG
#undef UDP_DRIVER_LOG
#undef TSCB_LOG
#undef SLW_LOG
#undef SFR_LOG
#undef SLR_LOG
#undef THREAD_POOL_LOG
