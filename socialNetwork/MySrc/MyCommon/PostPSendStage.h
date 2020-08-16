
#ifndef SOCIAL_NETWORK_MICROSERVICES_POSTPSENDSTAGE_H
#define SOCIAL_NETWORK_MICROSERVICES_POSTPSENDSTAGE_H

#include <thread>
#include <iostream>

#include <thrift/protocol/TProtocol.h>

#include "readerwriterqueue.h"
#include "concurrentqueue.h"
#include "core_schedule.h"
#include "stopwatch.h"

#include "PostPSendStage.fwd.h"
#include "../gen-cpp/FakeComposePostService.fwd.h"
#include "../gen-cpp/MyUniqueIdService.fwd.h"

#include "PrePRecvStage.h"

using namespace moodycamel;

namespace my_social_network {

typedef struct SendReq {
  void* args;
  ::apache::thrift::protocol::TProtocol* oprot;
  ::apache::thrift::protocol::TProtocol* iprot;
  ReaderWriterQueue<int> *transportRQ;
  int seqid;
} SendReq;

typedef struct PostPReq {
  ::apache::thrift::protocol::TProtocol* oprot;
  int32_t seqid;
  void* result;
  void* ctx;
} PostPReq;


class PostPSendStage {

 public:
  PostPSendStage(PrePRecvStage* prepRecvStage){
    int coreId;
    thread_ = std::thread([this] {Run_();});
    coreId = PinToCore(&thread_);
    prepRecvStage_ = prepRecvStage;
    // std::cout << "Send thread pinned to core " << coreId << "." << std::endl;
  }

  ~PostPSendStage() {
    exit_flag_ = true;
    thread_.join();

    #ifdef SW
    sendSW_.post_process();
    std::cout << "Send: " << sendSW_.mean() << std::endl;
    postpSW_.post_process();
    std::cout << "PostP: " << postpSW_.mean() << std::endl;
    // combinedSW_.post_process();
    // std::cout << "PostPSend Combined: " << combinedSW_.mean() << std::endl;
    #endif
  }

  void EnqueuePostPReq(::apache::thrift::protocol::TProtocol* oprot,
                       int32_t seqid, void *result, void* ctx);

  void EnqueueSendReq(::apache::thrift::protocol::TProtocol* oprot,
                      void *args, ::apache::thrift::protocol::TProtocol* iprot,
                      ReaderWriterQueue<int> *transportRQ, int seqid);

  void* PeekPostP();
  void PostPCompletion(int &completion);

  void* PeekSend();
  void SendCompletion(int& completion);

 private:
  std::thread thread_;
  std::atomic<bool> exit_flag_{false};
  // ReaderWriterQueue<SendReq> sendRQ_;
  ConcurrentQueue<SendReq> sendRQ_;
  ReaderWriterQueue<int> sendCQ_;

  ConcurrentQueue<PostPReq> postpRQ_;
  BlockingReaderWriterQueue<int> postpCQ_;

  PrePRecvStage* prepRecvStage_;

  // int32_t cseqid = 0;

  #ifdef SW
  Stopwatch<std::chrono::nanoseconds> sendSW_;
  Stopwatch<std::chrono::nanoseconds> postpSW_;
  // Stopwatch<std::chrono::nanoseconds> combinedSW_;
  #endif


  void Run_();
  void Send_(FakeComposePostService_UploadUniqueId_args* args,
             ::apache::thrift::protocol::TProtocol* oprot,
             ::apache::thrift::protocol::TProtocol* iprot,
             int seqid);

  void PostProcess_(MyUniqueIdService_UploadUniqueId_result *result, int32_t* seqid, ::apache::thrift::protocol::TProtocol* oprot, void* ctx);
};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_POSTPSENDSTAGE_H