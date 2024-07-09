#pragma once

#include "rtps/entities/Reader.h"

namespace rtps {
class StatelessReader final : public Reader {
public:
  bool init(const TopicData &attributes);
  void newChange(const ReaderCacheChange &cacheChange) override;
  bool onNewHeartbeat(const SubmessageHeartbeat &msg,
                      const GuidPrefix_t &remotePrefix) override;
  bool addNewMatchedWriter(const WriterProxy &newProxy) override;
  bool onNewGapMessage(const SubmessageGap &msg,
                       const GuidPrefix_t &remotePrefix) override;
};

}
