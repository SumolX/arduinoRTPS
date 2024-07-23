/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#include "rtps/entities/StatelessReader.h"

#include "rtps/utils/Lock.h"
#include "rtps/utils/Log.h"

using rtps::StatelessReader;

#if SLR_VERBOSE && RTPS_GLOBAL_VERBOSE
#include "rtps/utils/strutils.h"
#ifndef SLR_LOG
#define SLR_LOG(...)                                                           \
  if (true) {                                                                  \
    printf("[StatelessReader %s] ", &m_attributes.topicName[0]);               \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  }
#endif
#else
#define SLR_LOG(...) //
#endif

bool StatelessReader::init(const TopicData &attributes) {
  if (!initMutex()) {
    return false;
  }

  m_proxies.clear();
  m_attributes = attributes;
  m_is_initialized_ = true;
  return true;
}

void StatelessReader::newChange(const ReaderCacheChange &cacheChange) {
  if (!m_is_initialized_) {
    return;
  }
  executeCallbacks(cacheChange);
}

bool StatelessReader::addNewMatchedWriter(const WriterProxy &newProxy) {
#if (SLR_VERBOSE && RTPS_GLOBAL_VERBOSE)
  char buffer[64];
  guid2Str(newProxy.remoteWriterGuid, buffer, sizeof(buffer));
  SLR_LOG("Adding WriterProxy: %s", buffer);
#endif
  return m_proxies.add(newProxy);
}

bool StatelessReader::onNewHeartbeat(const SubmessageHeartbeat &,
                                     const GuidPrefix_t &) {
  // nothing to do
  return true;
}

bool StatelessReader::onNewGapMessage(const SubmessageGap &msg,
                                      const GuidPrefix_t &remotePrefix) {
  return true;
}

#undef SLR_VERBOSE
