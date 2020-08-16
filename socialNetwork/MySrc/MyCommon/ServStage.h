
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
            int num_threads){
    
    iface_ = iface;
    postpSendStage_ = postpSendStage;
    num_threads_ = num_threads;

    
    threads_ = new std::thread[num_threads_];
    tokens_ = new ProducerToken*[num_threads_];
    #ifdef SW
    servSW_ = new Stopwatch<std::chrono::microseconds>[num_threads_];
    #endif

    int coreId;
    for (int t=0; t < num_threads_; t++){
      tokens_[t] = new ProducerToken(servRQ_);
      threads_[t] = std::thread([this, t] {Run_(t);});
      coreId = PinToCore(&threads_[t]);
      // std::cout << "Send thread pinned to core " << coreId << "." << std::endl;
    }
  }

  ~ServStage() {
    exit_flag_ = true;
    for (int t = 0; t < num_threads_; t++){
      threads_[t].join();
      delete tokens_[t];
      #ifdef SW
      servSW_[t].post_process();
      std::cout << "[" << t << "] Serv: " << servSW_[t].mean() << std::endl;
      #endif
    }
    delete[] tokens_;
    delete[] threads_;

    #ifdef SW
    delete[] servSW_;
    #endif
  }

  void EnqueueServReq(void *args,
                      void *result,
                      int32_t seqid,
                      ::apache::thrift::protocol::TProtocol* oprot,
                      void* ctx);


  static void ResetToken() {
    current_token = 0;
  }

  ProducerToken* GetServToken(int tid);

 private:
  std::thread *threads_;
  int num_threads_;
  ProducerToken **tokens_;
  static int current_token;

  std::atomic<bool> exit_flag_{false};
  // ReaderWriterQueue<ServReq> servRQ_;
  ConcurrentQueue<ServReq> servRQ_;
  
  // ReaderWriterQueue<int> servCQ_;

  ::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface_;

  PostPSendStage* postpSendStage_;

  #ifdef SW
  Stopwatch<std::chrono::microseconds> *servSW_;
  #endif


  void Run_(int tid);
  void Serv_(MyUniqueIdService_UploadUniqueId_args* args);
};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_SERVSTAGE_H