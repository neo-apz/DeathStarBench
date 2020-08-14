#include "ServStage.h"

#include "../gen-cpp/MyUniqueIdService.h"

namespace my_social_network {

  void ServStage::EnqueueServReq(void *args, void *result, int32_t seqid,
                                 ::apache::thrift::protocol::TProtocol* oprot, void* ctx) {
    ServReq serv_req = {args, result, seqid, oprot, ctx};
    servRQ_.enqueue(serv_req);
    std::cout << "servRQ_.enqueue."<< std::endl;
  }
  
  void ServStage::Run_(int tid) {
    ServReq req;
    MyUniqueIdService_UploadUniqueId_args* args;

    while (!exit_flag_) {
      if (servRQ_.try_dequeue(req)){
        #ifdef SW
        servSW_[tid].start();
        #endif
        
        args = (MyUniqueIdService_UploadUniqueId_args*) req.args;
        Serv_(args);
        delete args;
        postpSendStage_->EnqueuePostPReq(req.oprot, req.seqid, req.result, req.ctx);

        #ifdef SW
        servSW_[tid].stop();
        #endif
      }
    }
  }

  void ServStage::Serv_(MyUniqueIdService_UploadUniqueId_args* args) {
    iface_->UploadUniqueId(args->req_id, args->post_type);
    // std::cout << "In thread!" << std::endl;
    // servCQ_.enqueue(1);
    // std::cout << "servCQ_.enqueue."<< std::endl;
  }

}// namespace my_social_network