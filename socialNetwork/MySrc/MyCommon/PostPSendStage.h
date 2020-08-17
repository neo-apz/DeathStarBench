
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

#define TOKEN_SIZE 16
#define TOKEN_SIZE_MASK 0xF

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
    sendTokens = new ProducerToken*[TOKEN_SIZE];

    for (int i = 0; i < TOKEN_SIZE; i++){
      sendTokens[i] = new ProducerToken(sendRQ_);
    }

    int coreId;
    thread_ = std::thread([this] {Run_();});
    coreId = PinToCore(&thread_);
    prepRecvStage_ = prepRecvStage;
    std::cout << "PostP/Send thread pinned to core " << coreId << "." << std::endl;
  }

  ~PostPSendStage() {
    exit_flag_ = true;
    thread_.join();

    for (int i = 0; i < TOKEN_SIZE; i++){
      delete sendTokens[i];
    }

    delete[] sendTokens;

    #ifdef SWD
    sendSW_.post_process();
    std::cout << "Send(ns): " << sendSW_.mean() << std::endl;
    postpSW_.post_process();
    std::cout << "PostP(ns): " << postpSW_.mean() << std::endl;

    // sendToCSW_.post_process();
    // std::cout << "SendToC(ns): " << sendToCSW_.mean() << std::endl;

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
  ReaderWriterQueue<int> postpCQ_;

  PrePRecvStage* prepRecvStage_;

  ProducerToken **sendTokens;

  // int32_t cseqid = 0;

  #ifdef SWD
  Stopwatch<std::chrono::nanoseconds> sendSW_;
  Stopwatch<std::chrono::nanoseconds> postpSW_;
  // Stopwatch<std::chrono::nanoseconds> combinedSW_;

  // Stopwatch<std::chrono::nanoseconds> sendToCSW_;
  #endif


  void Run_();
  void Send_(FakeComposePostService_UploadUniqueId_args* args,
             ::apache::thrift::protocol::TProtocol* oprot,
             ::apache::thrift::protocol::TProtocol* iprot,
             int seqid);

  void PostProcess_(MyUniqueIdService_UploadUniqueId_result *result,
                    int32_t seqid,
                    ::apache::thrift::protocol::TProtocol* oprot,
                    void* ctx);
};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_POSTPSENDSTAGE_H