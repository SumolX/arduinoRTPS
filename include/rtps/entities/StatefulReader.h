#pragma once

#include "lwip/sys.h"
#include "rtps/communication/UdpDriver.h"
#include "rtps/config.h"
#include "rtps/entities/Reader.h"
#include "rtps/entities/WriterProxy.h"
#include "rtps/storages/MemoryPool.h"

namespace rtps {
struct SubmessageHeartbeat;

template <class NetworkDriver> class StatefulReaderT final : public Reader {
public:
  ~StatefulReaderT() override;
  bool init(const TopicData &attributes, NetworkDriver &driver);
  void newChange(const ReaderCacheChange &cacheChange) override;
  bool addNewMatchedWriter(const WriterProxy &newProxy) override;
  bool onNewHeartbeat(const SubmessageHeartbeat &msg,
                      const GuidPrefix_t &remotePrefix) override;
  bool onNewGapMessage(const SubmessageGap &msg,
                       const GuidPrefix_t &remotePrefix) override;

  bool sendPreemptiveAckNack(const WriterProxy &writer) override;

private:
  Ip4Port_t m_srcPort; // TODO intended for reuse but buffer not used as such
  NetworkDriver *m_transport;
};

using StatefulReader = StatefulReaderT<UdpDriver>;

}

#include "StatefulReader.tpp"
