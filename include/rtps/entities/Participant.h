#pragma once

#include "rtps/common/types.h"
#include "rtps/config.h"
#include "rtps/discovery/SEDPAgent.h"
#include "rtps/discovery/SPDPAgent.h"
#include "rtps/messages/MessageReceiver.h"

namespace rtps {

class Writer;
class Reader;

class Participant {
public:
  GuidPrefix_t m_guidPrefix;
  ParticipantId_t m_participantId;

  Participant();
  explicit Participant(const GuidPrefix_t &guidPrefix,
                       ParticipantId_t participantId);

  // Not allowed because the message receiver contains a pointer to the
  // participant
  Participant(const Participant &) = delete;
  Participant(Participant &&) = delete;
  Participant &operator=(const Participant &) = delete;
  Participant &operator=(Participant &&) = delete;

  ~Participant();
  bool isValid();

  void reuse(const GuidPrefix_t &guidPrefix, ParticipantId_t participantId);

  std::array<uint8_t, 3> getNextUserEntityKey();

  // Actually the only two function that should be used by the user
  bool registerOnNewPublisherMatchedCallback(void (*callback)(void *arg),
                                             void *args);
  bool registerOnNewSubscriberMatchedCallback(void (*callback)(void *arg),
                                              void *args);

  //! Not-thread-safe function to add a writer
  Writer *addWriter(Writer *writer);
  bool isWritersFull();
  bool deleteWriter(Writer *writer);

  //! Not-thread-safe function to add a reader
  Reader *addReader(Reader *reader);
  bool isReadersFull();
  bool deleteReader(Reader *reader);

  //! (Probably) Thread safe if writers cannot be removed
  Writer *getWriter(EntityId_t id);
  Writer *getMatchingWriter(const TopicData &topicData);
  Writer *getMatchingWriter(const TopicDataCompressed &topicData);

  //! (Probably) Thread safe if readers cannot be removed
  Reader *getReader(EntityId_t id);
  Reader *getReaderByWriterId(const Guid_t &guid);
  Reader *getMatchingReader(const TopicData &topicData);
  Reader *getMatchingReader(const TopicDataCompressed &topicData);

  bool addNewRemoteParticipant(const ParticipantProxyData &remotePart);
  bool removeRemoteParticipant(const GuidPrefix_t &prefix);
  void removeAllProxiesOfParticipant(const GuidPrefix_t &prefix);
  void removeProxyFromAllEndpoints(const Guid_t &guid);

  const ParticipantProxyData *findRemoteParticipant(const GuidPrefix_t &prefix);
  void refreshRemoteParticipantLiveliness(const GuidPrefix_t &prefix);
  uint32_t getRemoteParticipantCount();
  MessageReceiver *getMessageReceiver();
  bool checkAndResetHeartbeats();

  bool hasReaderWithMulticastLocator(ip4_addr_t address);

  void addBuiltInEndpoints(BuiltInEndpoints &endpoints);
  void newMessage(const uint8_t *data, DataSize_t size);

  SPDPAgent &getSPDPAgent();
  void printInfo();

private:
  friend class SizeInspector;
  MessageReceiver m_receiver;
  bool m_hasBuilInEndpoints = false;
  std::array<uint8_t, 3> m_nextUserEntityId{{0, 0, 1}};
  std::array<Writer *, Config::NUM_WRITERS_PER_PARTICIPANT> m_writers = {
      nullptr};
  std::array<Reader *, Config::NUM_READERS_PER_PARTICIPANT> m_readers = {
      nullptr};

  SemaphoreHandle_t m_mutex;
  MemoryPool<ParticipantProxyData, Config::SPDP_MAX_NUMBER_FOUND_PARTICIPANTS>
      m_remoteParticipants;

  SPDPAgent m_spdpAgent;
  SEDPAgent m_sedpAgent;
};

}
