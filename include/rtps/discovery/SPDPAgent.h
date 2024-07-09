#pragma once

#include "lwip/sys.h"
#include "rtps/common/types.h"
#include "rtps/config.h"
#include "rtps/discovery/BuiltInEndpoints.h"
#include "rtps/discovery/ParticipantProxyData.h"
#include "rtps/utils/Log.h"
#include "ucdr/microcdr.h"

#if SPDP_VERBOSE && RTPS_GLOBAL_VERBOSE
#ifndef SPDP_LOG
#include "rtps/utils/strutils.h"
#define SPDP_LOG(...)                                                          \
  if (true) {                                                                  \
    printf("[SPDP] ");                                                         \
    printf(__VA_ARGS__);                                                       \
    printf("\r\n");                                                              \
  }
#endif
#else
#define SPDP_LOG(...) //
#endif

namespace rtps {
class Participant;
class Writer;
class Reader;
class ReaderCacheChange;

class SPDPAgent {
public:
  void init(Participant &participant, BuiltInEndpoints &endpoints);
  void start();
  void stop();
  SemaphoreHandle_t m_mutex;

private:
  Participant *mp_participant = nullptr;
  BuiltInEndpoints m_buildInEndpoints;
  bool m_running = false;
  std::array<uint8_t, 400> m_outputBuffer{}; // TODO check required size
  std::array<uint8_t, 400> m_inputBuffer{};
  ParticipantProxyData m_proxyDataBuffer{};
  ucdrBuffer m_microbuffer{};
  uint8_t m_cycleHB = 0;

  bool initialized = false;
  static void receiveCallback(void *callee,
                              const ReaderCacheChange &cacheChange);
  void handleSPDPPackage(const ReaderCacheChange &cacheChange);
  void configureEndianessAndOptions(ucdrBuffer &buffer);
  void processProxyData();
  bool addProxiesForBuiltInEndpoints();

  void addInlineQos();
  void addParticipantParameters();
  void endCurrentList();

  static void runBroadcast(void *args);
};

}
