
#ifndef SOCIAL_NETWORK_MICROSERVICES_SERVSTAGE_H
#define SOCIAL_NETWORK_MICROSERVICES_SERVSTAGE_H

#include <thread>
#include <iostream>

#include <thrift/protocol/TProtocol.h>

#include "readerwriterqueue.h"
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
            PostPSendStage* postpSendStage){
    
    iface_ = iface;
    postpSendStage_ = postpSendStage;

    int coreId;
    thread_ = std::thread([this] {Run_();});
    coreId = PinToCore(&thread_);
    // std::cout << "Send thread pinned to core " << coreId << "." << std::endl;
  }

  ~ServStage() {
    exit_flag_ = true;
    thread_.join();

    #ifdef SW
    servSW_.post_process();
    std::cout << "Serv: " << servSW_.mean() << std::endl;
    #endif
  }

  void EnqueueServReq(void *args,
                      void *result,
                      int32_t seqid,
                      ::apache::thrift::protocol::TProtocol* oprot,
                      void* ctx);


 private:
  std::thread thread_;
  std::atomic<bool> exit_flag_{false};
  ReaderWriterQueue<ServReq> servRQ_;
  // ReaderWriterQueue<int> servCQ_;

  ::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface_;

  PostPSendStage* postpSendStage_;

  #ifdef SW
  Stopwatch<std::chrono::nanoseconds> servSW_;
  #endif


  void Run_();
  void Serv_(MyUniqueIdService_UploadUniqueId_args* args);
};

} // namespace my_social_network


#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_SERVSTAGE_H