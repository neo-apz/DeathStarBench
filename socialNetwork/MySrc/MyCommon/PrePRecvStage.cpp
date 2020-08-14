
#include "PrePRecvStage.h"
#include "../gen-cpp/FakeComposePostService.h"
#include "../gen-cpp/MyUniqueIdService.h"


namespace my_social_network {

void PrePRecvStage::EnqueuePrePReq(apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> iprot,
                                   apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> oprot){
  PrePReq req = {iprot, oprot};
  prepRQ_.enqueue(req);
  std::cout << "prepRQ_.enqueue."<< std::endl;
}

void PrePRecvStage::EnqueueRecvReq(::apache::thrift::protocol::TProtocol* iprot, void *result){
  RecvReq req = {iprot, result};
  recvRQ_.enqueue(req);
  std::cout << "recvRQ_.enqueue."<< std::endl;
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

void PrePRecvStage::Run_(){
  int completion;
  RecvReq recvReq;
  PrePReq prepReq;

  FakeComposePostService_UploadUniqueId_presult *result;

  while (!exit_flag_){
    if (prepRQ_.peek() != nullptr){
      #ifdef SW
      prepSW_.start();
      #endif
      prepRQ_.try_dequeue(prepReq);
      _processor->process(prepReq.iprot, prepReq.oprot, nullptr);
      // prepCQ_.enqueue(1);
      #ifdef SW
      prepSW_.stop();
      #endif
    }

    if (recvRQ_.peek() != nullptr){
      #ifdef SW
      recvSW_.start();
      #endif
      recvRQ_.try_dequeue(recvReq);

      result = (FakeComposePostService_UploadUniqueId_presult* ) recvReq.result;
      Recv_(recvReq.iprot, result);
      #ifdef SW
      recvSW_.stop();
      #endif
    }
  }

  return; // exit!

}

void PrePRecvStage::PreProcess_() {

}

void PrePRecvStage::Recv_(::apache::thrift::protocol::TProtocol* iprot,
                          FakeComposePostService_UploadUniqueId_presult* result) {
  iprot->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
      ::apache::thrift::TApplicationException x;
      x.read(iprot);
      iprot->readMessageEnd();
      iprot->getTransport()->readEnd();
      throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
      iprot->skip(::apache::thrift::protocol::T_STRUCT);
      iprot->readMessageEnd();
      iprot->getTransport()->readEnd();
  }
  if (fname.compare("UploadUniqueId") != 0) {
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
  std::cout << "recvCQ_.enqueue."<< std::endl;
}

} // namespace my_social_network
