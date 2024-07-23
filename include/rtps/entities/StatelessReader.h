/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

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
