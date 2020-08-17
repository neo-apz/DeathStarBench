#include "ServStage.h"

#include "../gen-cpp/MyUniqueIdService.h"

namespace my_social_network {

int ServStage::current_token = 0;

void ServStage::EnqueueServReq(void *args, void *result, int32_t seqid,
                               ::apache::thrift::protocol::TProtocol* oprot, void* ctx) {
  ServReq serv_req = {args, result, seqid, oprot, ctx};
  servRQ_.enqueue(serv_req);
  // current_token = (current_token + 1) % num_threads_;
  // std::cout << "servRQ_.enqueue."<< std::endl;
}

ProducerToken* ServStage::GetServToken(int tid) {
    return tokens_[tid];
}

// static void ServStage::ResetToken(){
//   current_token = 0;
// }

void ServStage::Run_(int tid) {
  ServReq req;
  MyUniqueIdService_UploadUniqueId_args* args;

  while (!exit_flag_) {
    // std::cout << "servRQ_ size: " << servRQ_.size_approx() << std::endl;
    if (servRQ_.try_dequeue(req)){
      #ifdef SWD
      servSW_[tid].start();
      #endif
      
      args = (MyUniqueIdService_UploadUniqueId_args*) req.args;
      Serv_(args);

      #ifdef SWD
      servSW_[tid].stop();
      #endif
      postpSendStage_->EnqueuePostPReq(req.oprot, req.seqid, req.result, req.ctx);
      // delete args;
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