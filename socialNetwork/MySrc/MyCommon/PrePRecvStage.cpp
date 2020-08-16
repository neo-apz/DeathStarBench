
#include "PrePRecvStage.h"
#include "../gen-cpp/FakeComposePostService.h"
#include "../gen-cpp/MyUniqueIdService.h"


namespace my_social_network {

int PrePRecvStage::current_token = 0;

PrePRecvStage::PrePRecvStage(int num_threads){
    num_threads_ = num_threads;
    threads_ = new std::thread[num_threads_];
    prepTokens_ = new ProducerToken*[num_threads_];
    servTokens_ = new ProducerToken*[num_threads_];
    localData = new LocalData*[num_threads_];

    #ifdef SW
    recvSW_ = new Stopwatch<std::chrono::nanoseconds>[num_threads_];
    prepSW_ = new Stopwatch<std::chrono::nanoseconds>[num_threads_];
    #endif

    int coreId;
    for (int t=0; t < num_threads_; t++){
      localData[t] = new LocalData();
      prepTokens_[t] = new ProducerToken(prepRQ_);
      servTokens_[t] = _processor->_servStageHandler->GetServToken(t);
      
      threads_[t] = std::thread([this, t] {Run_(t);});
      coreId = PinToCore(&threads_[t]);
      // std::cout << "Send thread pinned to core " << coreId << "." << std::endl;
    }
}

void PrePRecvStage::EnqueuePrePReq(apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> iprot,
                                   apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> oprot){
  PrePReq req = {iprot, oprot};
  prepRQ_.enqueue(*prepTokens_[current_token], req);
  // std::cout << "prepRQ_.enqueue."<< std::endl;
  current_token = (current_token + 1) % num_threads_;
}

void PrePRecvStage::EnqueueRecvReq(::apache::thrift::protocol::TProtocol* iprot, void *result){
  RecvReq req = {iprot, result};
  recvRQ_.enqueue(req);
  // std::cout << "recvRQ_.enqueue."<< std::endl;
}

// void* PrePRecvStage::PeekRecv(){
//   return recvCQ_.peek();
// }

bool PrePRecvStage::RecvCompletion(int& completion){
  return recvCQ_.try_dequeue(completion);
}

void PrePRecvStage::setProcessor(std::shared_ptr<MyUniqueIdServiceProcessor> processor) {
  _processor = processor;
}

void PrePRecvStage::Run_(int tid){
  int completion;
  RecvReq recvReq;
  PrePReq prepReq;

  FakeComposePostService_UploadUniqueId_presult *result;

  while (!exit_flag_){
    if (prepRQ_.try_dequeue_from_producer(*prepTokens_[tid], prepReq)){
      #ifdef SW
      prepSW_[tid].start();
      #endif
      _processor->process(prepReq.iprot, prepReq.oprot, nullptr);
      // prepCQ_.enqueue(1);
      #ifdef SW
      prepSW_[tid].stop();
      #endif
    }

    if (recvRQ_.try_dequeue(recvReq)){
      #ifdef SW
      recvSW_[tid].start();
      #endif

      result = (FakeComposePostService_UploadUniqueId_presult* ) recvReq.result;
      Recv_(recvReq.iprot, result, tid);
      #ifdef SW
      recvSW_[tid].stop();
      #endif
    }
  }

  return; // exit!

}

void PrePRecvStage::PreProcess_() {

}

void PrePRecvStage::Recv_(::apache::thrift::protocol::TProtocol* iprot,
                          FakeComposePostService_UploadUniqueId_presult* result,
                          int tid) {
  iprot->readMessageBegin(localData[tid]->fname, localData[tid]->mtype, localData[tid]->rseqid);
  if (localData[tid]->mtype == ::apache::thrift::protocol::T_EXCEPTION) {
      ::apache::thrift::TApplicationException x;
      x.read(iprot);
      iprot->readMessageEnd();
      iprot->getTransport()->readEnd();
      throw x;
  }
  if (localData[tid]->mtype != ::apache::thrift::protocol::T_REPLY) {
      iprot->skip(::apache::thrift::protocol::T_STRUCT);
      iprot->readMessageEnd();
      iprot->getTransport()->readEnd();
  }
  if (localData[tid]->fname.compare("UploadUniqueId") != 0) {
      iprot->skip(::apache::thrift::protocol::T_STRUCT);
      iprot->readMessageEnd();
      iprot->getTransport()->readEnd();
  }

  result->read(iprot);
  iprot->readMessageEnd();
  iprot->getTransport()->readEnd();
  
  if (result->__isset.se) {
      throw result->se;
  }
  
  // std::cout << "End of Recv!" << std::endl;
  recvCQ_.enqueue(1);
  delete result;
  // std::cout << "recvCQ_.enqueue."<< std::endl;
}

} // namespace my_social_network
