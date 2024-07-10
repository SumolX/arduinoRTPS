#pragma once

#include <IPAddress.h>

#include "rtps/common/types.h"

namespace rtps {

#define IS_LITTLE_ENDIAN 1

// define only if using FreeRTOS
#define OS_IS_FREERTOS
#define PLATFORM_ESP32 1

struct Config {
    /**
     *  Runtime Defaults
     */
    static IPAddress IP_ADDRESS;
    static GuidPrefix_t BASE_GUID_PREFIX;
    static uint8_t DOMAIN_ID;

    /**
     *  Compile Time Constants
     */
    static constexpr VendorId_t VENDOR_ID = {13, 37};
    static constexpr uint8_t NUM_STATELESS_WRITERS = 4;
    static constexpr uint8_t NUM_STATELESS_READERS = 4;
    static constexpr uint8_t NUM_STATEFUL_WRITERS = 4;
    static constexpr uint8_t NUM_STATEFUL_READERS = 4;
    static constexpr uint8_t MAX_NUM_PARTICIPANTS = 1;
    static constexpr uint8_t NUM_WRITERS_PER_PARTICIPANT = MAX_NUM_PARTICIPANTS * (NUM_STATELESS_WRITERS + NUM_STATEFUL_WRITERS);
    static constexpr uint8_t NUM_READERS_PER_PARTICIPANT = MAX_NUM_PARTICIPANTS * (NUM_STATELESS_READERS + NUM_STATEFUL_READERS);
    static constexpr uint8_t NUM_WRITER_PROXIES_PER_READER = 1;
    static constexpr uint8_t NUM_READER_PROXIES_PER_WRITER = 1;

    static constexpr uint8_t MAX_NUM_UNMATCHED_REMOTE_WRITERS = 15;
    static constexpr uint8_t MAX_NUM_UNMATCHED_REMOTE_READERS = 15;
        
    static constexpr uint8_t MAX_NUM_READER_CALLBACKS = NUM_READERS_PER_PARTICIPANT;

    static constexpr uint8_t HISTORY_SIZE_STATELESS = 2;
    static constexpr uint8_t HISTORY_SIZE_STATEFUL = 10;

    static constexpr uint8_t MAX_TYPENAME_LENGTH = 32;
    static constexpr uint8_t MAX_TOPICNAME_LENGTH = 32;

    static constexpr int HEARTBEAT_STACKSIZE = 4000;          // byte
    static constexpr int THREAD_POOL_WRITER_STACKSIZE = 4000; // byte
    static constexpr int THREAD_POOL_READER_STACKSIZE = 4000; // byte
    static constexpr uint16_t SPDP_WRITER_STACKSIZE = 4000;   // byte

    static constexpr uint16_t SF_WRITER_HB_PERIOD_MS = 4000;
    static constexpr uint16_t SPDP_RESEND_PERIOD_MS = 1000;
    static constexpr uint8_t SPDP_CYCLECOUNT_HEARTBEAT = 2; // skip x SPDP rounds before checking liveliness
    static constexpr uint8_t SPDP_WRITER_PRIO = 24;
    static constexpr uint8_t SPDP_MAX_NUMBER_FOUND_PARTICIPANTS = 10;
    static constexpr uint8_t SPDP_MAX_NUM_LOCATORS = 1;
    static constexpr Duration_t SPDP_DEFAULT_REMOTE_LEASE_DURATION = {5, 0}; // Default lease duration for remote participants, usually
                                                                             // overwritten by remote info
    static constexpr Duration_t SPDP_MAX_REMOTE_LEASE_DURATION = {180, 0};   // Absolute maximum lease duration, ignoring remote participant info
    static constexpr Duration_t SPDP_LEASE_DURATION = {5, 0};

    static constexpr int MAX_NUM_UDP_CONNECTIONS = 10;

    static constexpr int THREAD_POOL_NUM_WRITERS = 1;
    static constexpr int THREAD_POOL_NUM_READERS = 1;
    static constexpr int THREAD_POOL_WRITER_PRIO = 24;
    static constexpr int THREAD_POOL_READER_PRIO = 24;
    static constexpr int THREAD_POOL_WORKLOAD_QUEUE_LENGTH_USERTRAFFIC = 60;
    static constexpr int THREAD_POOL_WORKLOAD_QUEUE_LENGTH_METATRAFFIC = 60;
    static constexpr int OVERALL_HEAP_SIZE =
        THREAD_POOL_NUM_WRITERS * THREAD_POOL_WRITER_STACKSIZE +
        THREAD_POOL_NUM_READERS * THREAD_POOL_READER_STACKSIZE +
        MAX_NUM_PARTICIPANTS * SPDP_WRITER_STACKSIZE +
        NUM_STATEFUL_WRITERS * HEARTBEAT_STACKSIZE;
};

};
