#pragma once

#include "lwip/sys.h"
#include "rtps/common/types.h"
#include "rtps/config.h"
#include "rtps/entities/Writer.h"
#include "rtps/storages/MemoryPool.h"
#include "rtps/storages/SimpleHistoryCache.h"

namespace rtps {

struct PBufWrapper;

template <typename NetworkDriver> class StatelessWriterT : public Writer {
public:
  ~StatelessWriterT() override;
  bool init(TopicData attributes, TopicKind_t topicKind, ThreadPool *threadPool,
            NetworkDriver &driver, bool enfUnicast = false);

  void progress() override;
  const CacheChange *newChange(ChangeKind_t kind, const uint8_t *data,
                               DataSize_t size, bool inLineQoS = false,
                               bool markDisposedAfterWrite = false) override;
  bool removeFromHistory(const SequenceNumber_t &s);

  void setAllChangesToUnsent() override;
  void onNewAckNack(const SubmessageAckNack &msg,
                    const GuidPrefix_t &sourceGuidPrefix) override;
  void reset() override;

private:
  NetworkDriver *m_transport;

  SimpleHistoryCache<Config::HISTORY_SIZE_STATELESS> m_history;
};

using StatelessWriter = StatelessWriterT<UdpDriver>;

}

#include "StatelessWriter.tpp"
