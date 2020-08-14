#include "ServStage.h"

#include "../gen-cpp/MyUniqueIdService.h"

namespace my_social_network {

  void ServStage::EnqueueServReq(void *args, void *result, int32_t seqid,
                                 ::apache::thrift::protocol::TProtocol* oprot, void* ctx) {
    ServReq serv_req = {args, result, seqid, oprot, ctx};
    servRQ_.enqueue(serv_req);
  }
  
  void ServStage::Run_() {
    ServReq req;
    MyUniqueIdService_UploadUniqueId_args* args;

    while (!exit_flag_) {
      if (servRQ_.peek() != nullptr){
        #ifdef SW
        servSW_.start();
        #endif

        servRQ_.try_dequeue(req);
        args = (MyUniqueIdService_UploadUniqueId_args*) req.args;
        Serv_(args);
        delete args;
        postpSendStage_->EnqueuePostPReq(req.oprot, req.seqid, req.result, req.ctx);

        #ifdef SW
        servSW_.stop();
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