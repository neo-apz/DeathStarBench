
#include "PrePRecvStage.h"
#include "../gen-cpp/FakeComposePostService.h"
// #include "../gen-cpp/MyUniqueIdService.h"


namespace my_social_network {

// void PrePRecvStage::Run_(){
//   int completion;
//   RecvReq req;

//   FakeComposePostService_UploadUniqueId_presult *result;

//   while (!exit_flag_){
//     if (sendRQ_.peek() != nullptr){
//       sendRQ_.try_dequeue(sendReq);
//       args = (FakeComposePostService_UploadUniqueId_args*) sendReq.args;
//       Send_(args, sendReq.oprot);
//       sendCQ_.enqueue(1);
//     }

//     if (postpRQ_.peek() != nullptr && servCQ_->peek() != nullptr){ // also check for completion of the function
//       postpRQ_.try_dequeue(postpReq);
//       servCQ_->try_dequeue(completion);
//       result = (MyUniqueIdService_UploadUniqueId_result* ) postpReq.result;
//       PostProcess_(result, &(postpReq.seqid), postpReq.oprot, postpReq.ctx);
//       postpCQ_.enqueue(1);
//     }
//   }

//   return; // exit!

// }

void PrePRecvStage::EnqueueRecvReq(::apache::thrift::protocol::TProtocol* iprot, void *result){
  RecvReq req = {iprot, result};
  recvRQ_.enqueue(req);
//   std::cout << "recvRQ_.enqueue."<< std::endl;
}

void* PrePRecvStage::PeekRecv(){
  return recvCQ_.peek();
}

void PrePRecvStage::RecvCompletion(int completion){
  recvCQ_.try_dequeue(completion);
}

void PrePRecvStage::Recv() {
  if (recvRQ_.peek() != nullptr) {
    recvRQ_.try_dequeue(req);

    req.iprot->readMessageBegin(fname, mtype, rseqid);
    if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
        ::apache::thrift::TApplicationException x;
        x.read(req.iprot);
        req.iprot->readMessageEnd();
        req.iprot->getTransport()->readEnd();
        throw x;
    }
    if (mtype != ::apache::thrift::protocol::T_REPLY) {
        req.iprot->skip(::apache::thrift::protocol::T_STRUCT);
        req.iprot->readMessageEnd();
        req.iprot->getTransport()->readEnd();
    }
    if (fname.compare("UploadUniqueId") != 0) {
        req.iprot->skip(::apache::thrift::protocol::T_STRUCT);
        req.iprot->readMessageEnd();
        req.iprot->getTransport()->readEnd();
    }
    result = (FakeComposePostService_UploadUniqueId_presult*) req.result;
    result->read(req.iprot);
    req.iprot->readMessageEnd();
    req.iprot->getTransport()->readEnd();
    
    if (result->__isset.se) {
        throw result->se;
    }
    
    // std::cout << "End of Recv!" << std::endl;
    recvCQ_.enqueue(1);
    delete result;
    // std::cout << "recvCQ_.enqueue."<< std::endl;
  }
}

} // namespace my_social_network
