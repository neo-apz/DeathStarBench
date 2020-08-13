
#ifndef SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H
#define SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H

#include <thread>
#include <iostream>

#include <thrift/protocol/TProtocol.h>

#include "readerwriterqueue.h"
#include "core_schedule.h"
#include "stopwatch.h"

#include "PrePRecvStage.fwd.h"
#include "../gen-cpp/FakeComposePostService.fwd.h"
// #include "../gen-cpp/MyUniqueIdService.fwd.h"

// #include "../gen-cpp/FakeComposePostService.h"
// #include "../gen-cpp/MyUniqueIdService.h"

using namespace moodycamel;

namespace my_social_network {

typedef struct RecvReq {
  ::apache::thrift::protocol::TProtocol* iprot;
  void* result;
} RecvReq;

class PrePRecvStage {

 public:
  PrePRecvStage(){
    // int coreId;
    // thread_ = std::thread([this] {Run_();});
    // coreId = PinToCore(&thread_);
    // std::cout << "Send thread pinned to core " << coreId << "." << std::endl;
  }

  ~PrePRecvStage() {
    // exit_flag_ = true;
    // thread_.join();

    #ifdef SW
    // sendSW_.post_process();
    // std::cout << "Send: " << sendSW_.mean() << std::endl;
    // postpSW_.post_process();
    // std::cout << "PostP: " << postpSW_.mean() << std::endl;
    #endif
  }

  void Recv();

//   void setSendCQ(ReaderWriterQueue<int>*);

//   void EnqueuePostPReq(::apache::thrift::protocol::TProtocol* oprot, int32_t seqid, void *result, void* ctx);
  void EnqueueRecvReq(::apache::thrift::protocol::TProtocol* iprot, void *result);

//   void* PeekPostP();
//   void PostPCompletion(int completion);

  void* PeekRecv();
  void RecvCompletion(int completion);

 private:
//   std::thread thread_;
//   std::atomic<bool> exit_flag_{false};
  ReaderWriterQueue<RecvReq> recvRQ_;
  ReaderWriterQueue<int> recvCQ_;

//   ReaderWriterQueue<int> *sendCQ_;
  
  int completion;
  RecvReq req;
  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;
  FakeComposePostService_UploadUniqueId_presult *result;

  #ifdef SW
//   Stopwatch<std::chrono::nanoseconds> sendSW_;
//   Stopwatch<std::chrono::nanoseconds> postpSW_;
  #endif


//   void Run_();
};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H