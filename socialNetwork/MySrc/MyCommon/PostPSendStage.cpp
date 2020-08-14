
#include "PostPSendStage.h"
#include "../gen-cpp/FakeComposePostService.h"
#include "../gen-cpp/MyUniqueIdService.h"

namespace my_social_network {

void PostPSendStage::EnqueuePostPReq(
  ::apache::thrift::protocol::TProtocol* oprot,
  int32_t seqid,
  void *result,
  void* ctx){

  PostPReq req = {oprot, seqid, result, ctx};
  postpRQ_.enqueue(req);
}

void* PostPSendStage::PeekPostP(){
  return postpCQ_.peek();
}

void PostPSendStage::PostPCompletion(int completion){
  postpCQ_.try_dequeue(completion);
}

void* PostPSendStage::PeekSend(){
  return sendCQ_.peek();
}

void PostPSendStage::SendCompetion(int completion){
  sendCQ_.try_dequeue(completion);
}

void PostPSendStage::EnqueueSendReq(::apache::thrift::protocol::TProtocol* oprot, void *args){
  SendReq req = {args, oprot};
  sendRQ_.enqueue(req);
}

void PostPSendStage::setServCQ(ReaderWriterQueue<int>* servCQ) {
  servCQ_ = servCQ;
}

void PostPSendStage::Run_() {
  SendReq sendReq;
  PostPReq postpReq;
  int completion;

  FakeComposePostService_UploadUniqueId_args* args;
  MyUniqueIdService_UploadUniqueId_result *result;

  while (!exit_flag_){
    if (sendRQ_.peek() != nullptr){
      sendRQ_.try_dequeue(sendReq);
      args = (FakeComposePostService_UploadUniqueId_args*) sendReq.args;
      Send_(args, sendReq.oprot);
      sendCQ_.enqueue(1);
    }

    if (postpRQ_.peek() != nullptr && servCQ_->peek() != nullptr){ // also check for completion of the function
      postpRQ_.try_dequeue(postpReq);
      servCQ_->try_dequeue(completion);
      result = (MyUniqueIdService_UploadUniqueId_result* ) postpReq.result;
      PostProcess_(result, &(postpReq.seqid), postpReq.oprot, postpReq.ctx);
      postpCQ_.enqueue(1);
    }
  }

  return; // exit!
}

void PostPSendStage::Send_(
  FakeComposePostService_UploadUniqueId_args* args,
  ::apache::thrift::protocol::TProtocol* oprot) {
  
  int32_t cseqid = 0;

  oprot->writeMessageBegin("UploadUniqueId", ::apache::thrift::protocol::T_CALL, cseqid);
  args->write(oprot);
  oprot->writeMessageEnd();
  oprot->getTransport()->writeEnd();
  oprot->getTransport()->flush();

  delete args;
  
  // std::cout << "End of Send, ReqGenPhase:" << isReqGenPhase << std::endl;

  // if (isReqGenPhase){
  //   _fakeProcessor->process(this->getOutputProtocol(), this->getInputProtocol(), nullptr);
  //   // std::cout << "End of UploadUniqueId, ReqGenPhase:" << isReqGenPhase << std::endl;
  //   continue;
  // }
}

void PostPSendStage::PostProcess_(
  MyUniqueIdService_UploadUniqueId_result *result,
  int32_t* seqid,
  ::apache::thrift::protocol::TProtocol* oprot,
  void* ctx) {
  
  oprot->writeMessageBegin("UploadUniqueId", ::apache::thrift::protocol::T_REPLY, *seqid);
  result->write(oprot);
  oprot->writeMessageEnd();
  uint32_t bytes = oprot->getTransport()->writeEnd();
  oprot->getTransport()->flush();

  delete result;

  // if (this->eventHandler_.get() != NULL) {
  //   this->eventHandler_->postWrite(req.ctx, "MyUniqueIdService.UploadUniqueId", bytes);
  // }
}


} // namespace my_social_network

