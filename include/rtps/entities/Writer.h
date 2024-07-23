/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "rtps/ThreadPool.h"
#include "rtps/discovery/TopicData.h"
#include "rtps/entities/ReaderProxy.h"
#include "rtps/storages/CacheChange.h"
#include "rtps/storages/MemoryPool.h"
#include "rtps/storages/PBufWrapper.h"

#ifdef DEBUG_BUILD
#define COMPILE_INIT_GUARD
#endif

#ifdef COMPILE_INIT_GUARD
#define INIT_GUARD()                                                           \
  if (!m_is_initialized_) {                                                    \
    while (1) {                                                                \
      printf("using uninitalized endpoint\n;");                                \
    }                                                                          \
  }
#else
#define INIT_GUARD() //
#endif

namespace rtps {

class Writer {
public:
  TopicData m_attributes;
  virtual bool addNewMatchedReader(const ReaderProxy &newProxy);
  virtual bool removeProxy(const Guid_t &guid);
  virtual void removeAllProxiesOfParticipant(const GuidPrefix_t &guidPrefix);
  virtual void reset() = 0;
  virtual const CacheChange *newChange(ChangeKind_t kind, const uint8_t *data,
                                       DataSize_t size);

  //! Executes required steps like sending packets. Intended to be called by
  //! worker threads
  virtual void progress() = 0;

  virtual bool removeFromHistory(const SequenceNumber_t &s) = 0;
  virtual void setAllChangesToUnsent() = 0;
  virtual void onNewAckNack(const SubmessageAckNack &msg,
                            const GuidPrefix_t &sourceGuidPrefix) = 0;

  using dumpProxyCallback = void (*)(const Writer *writer, const ReaderProxy &,
                                     void *arg);

  int dumpAllProxies(dumpProxyCallback target, void *arg);

  bool isInitialized();
  uint32_t getProxiesCount();

  void setSEDPSequenceNumber(const SequenceNumber_t &sn);
  const SequenceNumber_t &getSEDPSequenceNumber();

  bool isBuiltinEndpoint();

protected:
  SequenceNumber_t m_sedp_sequence_number;

  SemaphoreHandle_t m_mutex = nullptr;
  ThreadPool *mp_threadPool = nullptr;

  Ip4Port_t m_srcPort;

  bool m_enforceUnicast;

  TopicKind_t m_topicKind = TopicKind_t::NO_KEY;
  SequenceNumber_t m_nextSequenceNumberToSend;

  friend class SEDPAgent;
  virtual const CacheChange *newChange(ChangeKind_t kind, const uint8_t *data,
                                       DataSize_t size, bool inLineQoS,
                                       bool markDisposedAfterWrite) = 0;

  friend class SizeInspector;
  bool m_is_initialized_ = false;
  virtual ~Writer() = default;
  MemoryPool<ReaderProxy, Config::NUM_READER_PROXIES_PER_WRITER> m_proxies;

  void resetSendOptions();
  void manageSendOptions();
  bool isIrrelevant(ChangeKind_t kind) const;
};

}
