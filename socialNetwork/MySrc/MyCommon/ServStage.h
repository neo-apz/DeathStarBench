
#ifndef SOCIAL_NETWORK_MICROSERVICES_SERVSTAGE_H
#define SOCIAL_NETWORK_MICROSERVICES_SERVSTAGE_H

#include <thread>
#include <iostream>

#include <thrift/protocol/TProtocol.h>

// #include "readerwriterqueue.h"
#include "concurrentqueue.h"
#include "core_schedule.h"
#include "stopwatch.h"

#include "../gen-cpp/MyUniqueIdService.fwd.h"

#include "PostPSendStage.h"

using namespace moodycamel;

namespace my_social_network {

typedef struct ServReq {
  void* args;
  void* result;
  int32_t seqid;
  ::apache::thrift::protocol::TProtocol* oprot;
  void* ctx;
} ServReq;

class ServStage {

 public:
  ServStage(::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface,
            PostPSendStage* postpSendStage,
            int num_treads){
    
    iface_ = iface;
    postpSendStage_ = postpSendStage;
    num_treads_ = num_treads;

    int coreId;
    // threads_ = (std::thread*) malloc(sizeof(std::thread) * num_treads_);
    for (int t=0; t < num_treads_; t++){
      threads_[t] = std::thread([this, t] {Run_(t);});
      coreId = PinToCore(&threads_[t]);
      // std::cout << "Send thread pinned to core " << coreId << "." << std::endl;
    }

    // #ifdef SW
    // servSW_ = (Stopwatch<std::chrono::nanoseconds>*) malloc(sizeof(Stopwatch<std::chrono::nanoseconds>) * num_treads_);
    // #endif
  }

  ~ServStage() {
    exit_flag_ = true;
    for (int t = 0; t < num_treads_; t++){
      threads_[t].join();
      #ifdef SW
      servSW_[t].post_process();
      std::cout << "[" << t << "] Serv: " << servSW_[t].mean() << std::endl;
      #endif
    }

    #ifdef SW
    // free(servSW_);
    #endif
  }

  void EnqueueServReq(void *args,
                      void *result,
                      int32_t seqid,
                      ::apache::thrift::protocol::TProtocol* oprot,
                      void* ctx);


 private:
  std::thread threads_[3];
  int num_treads_;
  std::atomic<bool> exit_flag_{false};
  // ReaderWriterQueue<ServReq> servRQ_;
  ConcurrentQueue<ServReq> servRQ_;
  
  // ReaderWriterQueue<int> servCQ_;

  ::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface_;

  PostPSendStage* postpSendStage_;

  #ifdef SW
  Stopwatch<std::chrono::nanoseconds> servSW_[3];
  #endif


  void Run_(int tid);
  void Serv_(MyUniqueIdService_UploadUniqueId_args* args);
};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_SERVSTAGE_H