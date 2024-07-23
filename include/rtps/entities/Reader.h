/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include <cstring>

#include "rtps/common/types.h"
#include "rtps/config.h"
#include "rtps/discovery/TopicData.h"
#include "rtps/entities/WriterProxy.h"
#include "rtps/storages/MemoryPool.h"
#include "rtps/storages/PBufWrapper.h"
#include "semphr.h"


namespace rtps {

struct SubmessageHeartbeat;
struct SubmessageGap;

class ReaderCacheChange {
private:
  const uint8_t *data;

public:
  const ChangeKind_t kind;
  const DataSize_t size;
  const Guid_t writerGuid;
  const SequenceNumber_t sn;

  ReaderCacheChange(ChangeKind_t kind, Guid_t &writerGuid, SequenceNumber_t sn,
                    const uint8_t *data, DataSize_t size)
      : data(data), kind(kind), size(size), writerGuid(writerGuid), sn(sn){};

  ~ReaderCacheChange() =
      default; // No need to free data. It's not owned by this object
  // Not allowed because this class doesn't own the ptr and the user isn't
  // allowed to use it outside the Scope of the callback
  ReaderCacheChange(const ReaderCacheChange &other) = delete;
  ReaderCacheChange(ReaderCacheChange &&other) = delete;
  ReaderCacheChange &operator=(const ReaderCacheChange &other) = delete;
  ReaderCacheChange &operator=(ReaderCacheChange &&other) = delete;

  bool copyInto(uint8_t *buffer, DataSize_t destSize) const {
    if (destSize < size) {
      return false;
    } else {
      memcpy(buffer, data, size);
      return true;
    }
  }

  const uint8_t *getData() const { return data; }

  const DataSize_t getDataSize() const { return size; }
};

typedef void (*ddsReaderCallback_fp)(void *callee,
                                     const ReaderCacheChange &cacheChange);

class Reader {
public:
  using callbackFunction_t = void (*)(void *, const ReaderCacheChange &);
  using callbackIdentifier_t = uint32_t;

  TopicData m_attributes;
  virtual void newChange(const ReaderCacheChange &cacheChange) = 0;
  virtual callbackIdentifier_t registerCallback(callbackFunction_t cb,
                                                void *arg);
  virtual bool removeCallback(callbackIdentifier_t identifier);
  uint8_t getNumCallbacks();

  virtual bool onNewHeartbeat(const SubmessageHeartbeat &msg,
                              const GuidPrefix_t &remotePrefix) = 0;
  virtual bool onNewGapMessage(const SubmessageGap &msg,
                               const GuidPrefix_t &remotePrefix) = 0;
  virtual bool addNewMatchedWriter(const WriterProxy &newProxy) = 0;
  virtual bool removeProxy(const Guid_t &guid);
  virtual void removeAllProxiesOfParticipant(const GuidPrefix_t &guidPrefix);
  bool isInitialized() { return m_is_initialized_; }
  virtual void reset();
  bool isProxy(const Guid_t &guid);
  WriterProxy *getProxy(Guid_t guid);
  uint32_t getProxiesCount();

  void setSEDPSequenceNumber(const SequenceNumber_t &sn);
  const SequenceNumber_t &getSEDPSequenceNumber();

  using dumpProxyCallback = void (*)(const Reader *reader, const WriterProxy &,
                                     void *arg);

  int dumpAllProxies(dumpProxyCallback target, void *arg);

  virtual bool sendPreemptiveAckNack(const WriterProxy &writer);

protected:
  void executeCallbacks(const ReaderCacheChange &cacheChange);
  bool initMutex();

  SequenceNumber_t m_sedp_sequence_number;

  bool m_is_initialized_ = false;
  Reader();
  virtual ~Reader() = default;
  MemoryPool<WriterProxy, Config::NUM_WRITER_PROXIES_PER_READER> m_proxies;

  callbackIdentifier_t m_callback_identifier = 1;

  uint8_t m_callback_count = 0;
  using callbackElement_t = struct {
    callbackFunction_t function;
    void *arg;
    callbackIdentifier_t identifier;
  };

  std::array<callbackElement_t, Config::MAX_NUM_READER_CALLBACKS> m_callbacks;

  // Guards manipulation of the proxies array
  SemaphoreHandle_t m_proxies_mutex = nullptr;

  // Guards manipulation of callback array
  SemaphoreHandle_t m_callback_mutex = nullptr;
};

}
