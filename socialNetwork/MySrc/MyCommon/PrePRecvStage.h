
#ifndef SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H
#define SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H

#include <thread>
#include <iostream>

#include <thrift/protocol/TProtocol.h>

#include "readerwriterqueue.h"
#include "concurrentqueue.h"
#include "core_schedule.h"
#include "stopwatch.h"

#include "PrePRecvStage.fwd.h"
#include "../gen-cpp/FakeComposePostService.fwd.h"
#include "../gen-cpp/MyUniqueIdService.fwd.h"

// #include "../gen-cpp/FakeComposePostService.h"
// #include "../gen-cpp/MyUniqueIdService.h"

using namespace moodycamel;

namespace my_social_network {

typedef struct RecvReq {
  ::apache::thrift::protocol::TProtocol* iprot;
  void* result;
} RecvReq;

typedef struct PrePReq {
  apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> iprot;
  apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> oprot;
} PrePReq;

class PrePRecvStage {

 public:
  PrePRecvStage(){
    int coreId;
    thread_ = std::thread([this] {Run_();});
    coreId = PinToCore(&thread_);
    // std::cout << "Send thread pinned to core " << coreId << "." << std::endl;
  }

  ~PrePRecvStage() {
    exit_flag_ = true;
    thread_.join();

    #ifdef SW
    recvSW_.post_process();
    std::cout << "Recv: " << recvSW_.mean() << std::endl;
    prepSW_.post_process();
    std::cout << "PreP: " << prepSW_.mean() << std::endl;
    #endif
  }

  void Run_();
  void PreProcess_();
  void Recv_(::apache::thrift::protocol::TProtocol* iprot, FakeComposePostService_UploadUniqueId_presult* result);

  void setProcessor(std::shared_ptr<MyUniqueIdServiceProcessor> processor);

  void EnqueuePrePReq(apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> iprot,
                      apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> oprot);
  void EnqueueRecvReq(::apache::thrift::protocol::TProtocol* iprot, void *result);

//   void* PeekPostP();
//   void PostPCompletion(int completion);

  void* PeekRecv();
  bool RecvCompletion(int& completion);

 private:
  std::thread thread_;
  std::atomic<bool> exit_flag_{false};
  ReaderWriterQueue<RecvReq> recvRQ_;
  ConcurrentQueue<int> recvCQ_;

  ReaderWriterQueue<PrePReq> prepRQ_;
  ReaderWriterQueue<int> prepCQ_;
  
  int completion;
  RecvReq req;
  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  std::shared_ptr<MyUniqueIdServiceProcessor> _processor;

  #ifdef SW
  Stopwatch<std::chrono::nanoseconds> recvSW_;
  Stopwatch<std::chrono::nanoseconds> prepSW_;
  #endif

};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H