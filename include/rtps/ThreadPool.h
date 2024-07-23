/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include <array>

#include "lwip/sys.h"
#include "rtps/communication/PacketInfo.h"
#include "rtps/communication/UdpDriver.h"
#include "rtps/config.h"
#include "rtps/storages/PBufWrapper.h"
#include "rtps/storages/ThreadSafeCircularBuffer.h"

namespace rtps {

class Writer;

class ThreadPool {
public:
  using receiveJumppad_fp = void (*)(void *callee, const PacketInfo &packet);

  ThreadPool(receiveJumppad_fp receiveCallback, void *callee);

  ~ThreadPool();

  bool startThreads();
  void stopThreads();

  void clearQueues();
  bool addWorkload(Writer *workload);
  bool addNewPacket(PacketInfo &&packet);

  static void readCallback(void *arg, udp_pcb *pcb, pbuf *p,
                           const ip_addr_t *addr, Ip4Port_t port);

  bool addBuiltinPort(const Ip4Port_t &port);

private:
  receiveJumppad_fp m_receiveJumppad;
  void *m_callee;
  bool m_running = false;
  std::array<sys_thread_t, Config::THREAD_POOL_NUM_WRITERS> m_writers;
  std::array<sys_thread_t, Config::THREAD_POOL_NUM_READERS> m_readers;

  std::array<Ip4Port_t, 2 * Config::MAX_NUM_PARTICIPANTS> m_builtinPorts;
  size_t m_builtinPortsIdx = 0;

  sys_sem_t m_readerNotificationSem;
  sys_sem_t m_writerNotificationSem;

  void updateDiagnostics();

  using BufferUsertrafficOutgoing = ThreadSafeCircularBuffer<
      Writer *, Config::THREAD_POOL_WORKLOAD_QUEUE_LENGTH_USERTRAFFIC>;
  using BufferMetatrafficOutgoing = ThreadSafeCircularBuffer<
      Writer *, Config::THREAD_POOL_WORKLOAD_QUEUE_LENGTH_METATRAFFIC>;
  using BufferUsertrafficIncoming = ThreadSafeCircularBuffer<
      PacketInfo, Config::THREAD_POOL_WORKLOAD_QUEUE_LENGTH_USERTRAFFIC>;
  using BufferMetatrafficIncoming = ThreadSafeCircularBuffer<
      PacketInfo, Config::THREAD_POOL_WORKLOAD_QUEUE_LENGTH_METATRAFFIC>;

  BufferUsertrafficOutgoing m_outgoingUserTraffic;
  BufferMetatrafficOutgoing m_outgoingMetaTraffic;

  BufferUsertrafficIncoming m_incomingUserTraffic;
  BufferMetatrafficIncoming m_incomingMetaTraffic;

  bool isBuiltinPort(const Ip4Port_t &port);
  static void writerThreadFunction(void *arg);
  static void readerThreadFunction(void *arg);
  void doWriterWork();
  void doReaderWork();
};

}
