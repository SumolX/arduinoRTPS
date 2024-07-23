/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "rtps/entities/ReaderProxy.h"
#include "rtps/entities/Writer.h"
#include "rtps/storages/HistoryCacheWithDeletion.h"
#include "rtps/storages/MemoryPool.h"

namespace rtps {

template <class NetworkDriver> class StatefulWriterT final : public Writer {
public:
  ~StatefulWriterT() override;
  bool init(TopicData attributes, TopicKind_t topicKind, ThreadPool *threadPool,
            NetworkDriver &driver, bool enfUnicast = false);

  //! Executes required steps like sending packets. Intended to be called by
  //! worker threads
  void progress() override;
  const CacheChange *newChange(ChangeKind_t kind, const uint8_t *data,
                               DataSize_t size, bool inLineQoS = false,
                               bool markDisposedAfterWrite = false) override;

  bool removeFromHistory(const SequenceNumber_t &s);
  void setAllChangesToUnsent() override;
  void onNewAckNack(const SubmessageAckNack &msg,
                    const GuidPrefix_t &sourceGuidPrefix) override;
  void reset() override;
  void updateChangeKind(SequenceNumber_t &sequence_number);

private:
  NetworkDriver *m_transport;

  HistoryCacheWithDeletion<Config::HISTORY_SIZE_STATEFUL> m_history;

  /*
   * Cache changes marked as disposeAfterWrite are retained for a short amount
   * in case of retransmission The whole 'disposeAfterWrite' mechanisms only
   * exists to allow for repeated creation and deletion of endpoints during
   * operation. Otherwise the history will quickly reach its limits. Will be
   * replaced with something more elegant in the future.
   */
  ThreadSafeCircularBuffer<SequenceNumber_t, 10> m_disposeWithDelay;
  void dropDisposeAfterWriteChanges();

  sys_thread_t m_heartbeatThread;

  Count_t m_hbCount{1};

  bool m_running = true;
  bool m_thread_running = false;

  bool sendData(const ReaderProxy &reader, const CacheChange *next);
  bool sendDataWRMulticast(const ReaderProxy &reader, const CacheChange *next);
  static void hbFunctionJumppad(void *thisPointer);
  void sendHeartBeatLoop();
  void sendHeartBeat();
  void sendGap(const ReaderProxy &reader, const SequenceNumber_t &firstMissing,
               const SequenceNumber_t &nextValid);
};

using StatefulWriter = StatefulWriterT<UdpDriver>;

}

#include "StatefulWriter.tpp"
