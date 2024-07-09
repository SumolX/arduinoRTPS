#pragma once

#include <stdint.h>

namespace rtps {
namespace Diagnostics {

namespace ThreadPool {
extern uint32_t dropped_incoming_packets_usertraffic;
extern uint32_t dropped_incoming_packets_metatraffic;

extern uint32_t dropped_outgoing_packets_usertraffic;
extern uint32_t dropped_outgoing_packets_metatraffic;

extern uint32_t processed_incoming_metatraffic;
extern uint32_t processed_outgoing_metatraffic;
extern uint32_t processed_incoming_usertraffic;
extern uint32_t processed_outgoing_usertraffic;

extern uint32_t max_ever_elements_outgoing_usertraffic_queue;
extern uint32_t max_ever_elements_incoming_usertraffic_queue;

extern uint32_t max_ever_elements_outgoing_metatraffic_queue;
extern uint32_t max_ever_elements_incoming_metatraffic_queue;
} // namespace ThreadPool

namespace StatefulReader {
extern uint32_t sfr_unexpected_sn;
extern uint32_t sfr_retransmit_requests;
} // namespace StatefulReader

namespace Network {
extern uint32_t lwip_allocation_failures;
}

namespace OS {
extern uint32_t current_free_heap_size;
}

namespace SEDP {
extern uint32_t max_ever_remote_participants;
extern uint32_t current_remote_participants;

extern uint32_t max_ever_matched_reader_proxies;
extern uint32_t current_max_matched_reader_proxies;

extern uint32_t max_ever_matched_writer_proxies;
extern uint32_t current_max_matched_writer_proxies;

extern uint32_t max_ever_unmatched_reader_proxies;
extern uint32_t current_max_unmatched_reader_proxies;

extern uint32_t max_ever_unmatched_writer_proxies;
extern uint32_t current_max_unmatched_writer_proxies;
} // namespace SEDP

}

}
