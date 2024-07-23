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
#include "rtps/config.h"
#include "rtps/entities/Participant.h"
#include "rtps/entities/StatefulReader.h"
#include "rtps/entities/StatefulWriter.h"
#include "rtps/entities/StatelessReader.h"
#include "rtps/entities/StatelessWriter.h"
#include "rtps/storages/PBufWrapper.h"
#include "rtps/common/Locator.h"

namespace rtps {
class Domain {
public:
  Domain();
  ~Domain();

  bool completeInit();
  void stop();

  Participant *createParticipant();
  Writer *createWriter(Participant &part, const char *topicName,
                       const char *typeName, bool reliable,
                       bool enforceUnicast = false);
  Reader *createReader(Participant &part, const char *topicName,
                       const char *typeName, bool reliable,
                       ip4_addr_t mcastaddress = {0});

  Writer *writerExists(Participant &part, const char *topicName,
                       const char *typeName, bool reliable);
  Reader *readerExists(Participant &part, const char *topicName,
                       const char *typeName, bool reliable);

  bool deleteWriter(Participant &part, Writer *writer);
  bool deleteReader(Participant &part, Reader *reader);

  void printInfo();

private:
  friend class SizeInspector;
  ThreadPool m_threadPool;
  UdpDriver m_transport;
  std::array<Participant, Config::MAX_NUM_PARTICIPANTS> m_participants;
  const uint8_t PARTICIPANT_START_ID = 0;
  ParticipantId_t m_nextParticipantId = PARTICIPANT_START_ID;

  std::array<StatelessWriter, Config::NUM_STATELESS_WRITERS> m_statelessWriters;
  std::array<StatelessReader, Config::NUM_STATELESS_READERS> m_statelessReaders;
  std::array<StatefulReader, Config::NUM_STATEFUL_READERS> m_statefulReaders;
  std::array<StatefulWriter, Config::NUM_STATEFUL_WRITERS> m_statefulWriters;
  template <typename A, typename B> B *getNextUnusedEndpoint(A &a) {
    for (unsigned int i = 0; i < a.size(); i++) {
      if (!a[i].isInitialized()) {
        return &(a[i]);
      }
    }
    return nullptr;
  }

  bool m_initComplete = false;
  SemaphoreHandle_t m_mutex;

  void receiveCallback(const PacketInfo &packet);
  GuidPrefix_t generateGuidPrefix(ParticipantId_t id) const;
  void createBuiltinWritersAndReaders(Participant &part);
  void registerPort(const Participant &part);
  void registerMulticastPort(FullLengthLocator mcastLocator);
  static void receiveJumppad(void *callee, const PacketInfo &packet);
};

}
