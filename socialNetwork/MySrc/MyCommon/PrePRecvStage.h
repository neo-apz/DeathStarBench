
#ifndef SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H
#define SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H

#include <thread>
#include <iostream>

#include <thrift/protocol/TProtocol.h>

// #include "readerwriterqueue.h"
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

typedef struct LocalData {
  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;
} LocalData;

class PrePRecvStage {

 public:
  PrePRecvStage(int num_threads);

  ~PrePRecvStage() {
    exit_flag_ = true;
    for (int t = 0; t < num_threads_; t++){
      threads_[t].join();
      // delete prepTokens_[t];
      delete localData[t];
      // delete servTokens_[t];
      #ifdef SW
      recvSW_[t].post_process();
      std::cout << "[" << t << "] Recv: " << recvSW_[t].mean() << std::endl;
      prepSW_[t].post_process();
      std::cout << "[" << t << "] PreP: " << prepSW_[t].mean() << std::endl;
      #endif
    }

    for (int i=0; i < 10; i++){
      delete tokens_[i];
    }

    // delete[] prepTokens_;
    delete[] threads_;
    delete[] localData;
    // delete[] servTokens_;

    #ifdef SW
    delete[] recvSW_;
    delete[] prepSW_;
    #endif
  }

  void Run_(int tid);
  void PreProcess_();
  void Recv_(::apache::thrift::protocol::TProtocol* iprot, FakeComposePostService_UploadUniqueId_presult* result, int tid);

  void setProcessor(MyUniqueIdServiceProcessor *processor);

  void EnqueuePrePReq(apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> iprot,
                      apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> oprot);
  void EnqueueRecvReq(::apache::thrift::protocol::TProtocol* iprot, void *result);

//   void* PeekPostP();
//   void PostPCompletion(int completion);

  void* PeekRecv();
  void RecvCompletion(int seqid);

  // static void ResetToken() {
  //   current_token = 0;
  // }

 private:
  std::thread *threads_;
  int num_threads_;
  ProducerToken *tokens_[10];
  // ProducerToken **servTokens_;
  // static int current_token;

  std::atomic<bool> exit_flag_{false};
  ConcurrentQueue<RecvReq> recvRQ_;
  ConcurrentQueue<int> recvCQ_;

  ConcurrentQueue<PrePReq> prepRQ_;
  // ReaderWriterQueue<int> prepCQ_;
  
  int completion;
  LocalData **localData;
  
  MyUniqueIdServiceProcessor* _processor;

  #ifdef SW
  Stopwatch<std::chrono::nanoseconds> *recvSW_;
  Stopwatch<std::chrono::nanoseconds> *prepSW_;
  #endif

};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_PREPRECVSTAGE_H