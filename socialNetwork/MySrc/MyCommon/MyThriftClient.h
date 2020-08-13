#ifndef SOCIAL_NETWORK_MICROSERVICES_MYTHRIFTCLIENT_H
#define SOCIAL_NETWORK_MICROSERVICES_MYTHRIFTCLIENT_H

#include <string>
#include <thread>
#include <iostream>
#include <boost/log/trivial.hpp>

// #include <thrift/transport/TBufferTransports.h>
#include "MyTMemoryBuffer.h"

#include "PostPSendStage.h"

#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>
// #include "logger.h"

#include "MyGenericClient.h"

namespace my_social_network {


using namespace ::apache::thrift;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::protocol;

// using apache::thrift::protocol::TProtocol;
// using apache::thrift::protocol::TBinaryProtocol;
// using apache::thrift::transport::TFramedTransport;
// using apache::thrift::transport::TSocket;
// using apache::thrift::transport::TTransport;
using apache::thrift::TException;

template<class TThriftClient>
class MyThriftClient : public MyGenericClient{
 public:
  MyThriftClient(uint32_t sz);
  
  #ifdef STAGED
  MyThriftClient(uint32_t sz, PostPSendStage* postpSendStageHandler);
  #endif

  MyThriftClient(uint8_t* cltIbuf, uint32_t ISz, uint8_t* cltObuf, uint32_t OSz);

  MyThriftClient(const MyThriftClient &) = delete;
  MyThriftClient &operator=(const MyThriftClient &) = delete;
  MyThriftClient(MyThriftClient<TThriftClient> &&) = default;
  MyThriftClient &operator=(MyThriftClient &&) = default;

  ~MyThriftClient() override;

  TThriftClient *GetClient() const;

  void GetBuffer(uint8_t** cltIBufPtr, uint32_t* ISz,
            uint8_t** cltOBufPtr, uint32_t* OSz);

  void WroteBytes(uint32_t len, bool input);
  void ReadBytes(uint32_t len, bool input);

  void Connect() override;
  void Disconnect() override;
  void KeepAlive() override;
  void KeepAlive(int timeout_ms) override;
  bool IsConnected() override;

  // #ifdef STAGED
  // PostPSendStage* _postpSendStageHandler;
  // #endif

 private:
  TThriftClient *_client;

  std::shared_ptr<MyTMemoryBuffer> _cltITransport;
  std::shared_ptr<MyTMemoryBuffer> _cltOTransport;

  std::shared_ptr<TCompactProtocol> _ctlIProt;
  std::shared_ptr<TCompactProtocol> _ctlOProt;

};

template<class TThriftClient>
MyThriftClient<TThriftClient>::MyThriftClient(uint32_t sz) {
  _cltITransport = std::make_shared<MyTMemoryBuffer>(sz);
  _cltOTransport = std::make_shared<MyTMemoryBuffer>(sz);

  _ctlIProt = std::make_shared<TCompactProtocol>(_cltITransport);
  _ctlOProt = std::make_shared<TCompactProtocol>(_cltOTransport);

  _client = new TThriftClient(_ctlIProt, _ctlOProt);
}

#ifdef STAGED
template<class TThriftClient>
MyThriftClient<TThriftClient>::MyThriftClient(uint32_t sz, PostPSendStage* postpSendStageHandler) {
  _cltITransport = std::make_shared<MyTMemoryBuffer>(sz);
  _cltOTransport = std::make_shared<MyTMemoryBuffer>(sz);

  _ctlIProt = std::make_shared<TCompactProtocol>(_cltITransport);
  _ctlOProt = std::make_shared<TCompactProtocol>(_cltOTransport);

  _client = new TThriftClient(_ctlIProt, _ctlOProt, postpSendStageHandler);
}
#endif

template<class TThriftClient>
MyThriftClient<TThriftClient>::MyThriftClient(uint8_t* cltIbuf, uint32_t ISz,
                                              uint8_t* cltObuf, uint32_t OSz) {
  _cltITransport = std::make_shared<MyTMemoryBuffer>(cltIbuf, ISz, MyTMemoryBuffer::TAKE_COOWNERSHIP);
  _cltOTransport = std::make_shared<MyTMemoryBuffer>(cltObuf, OSz, MyTMemoryBuffer::TAKE_COOWNERSHIP);

  _ctlIProt = std::make_shared<TCompactProtocol>(_cltITransport);
  _ctlOProt = std::make_shared<TCompactProtocol>(_cltOTransport);

  _client = new TThriftClient(_ctlIProt, _ctlOProt);
}


template<class TThriftClient>
void MyThriftClient<TThriftClient>::GetBuffer(uint8_t** cltIBufPtr, uint32_t* ISz,
                                              uint8_t** cltOBufPtr, uint32_t* OSz){
  _cltITransport->getBuffer(cltIBufPtr, ISz);
  _cltOTransport->getBuffer(cltOBufPtr, OSz);
}

template<class TThriftClient>
void MyThriftClient<TThriftClient>::WroteBytes(uint32_t len, bool input) {

  if (input)
    _cltITransport->wroteBytes(len);
  else
    _cltOTransport->wroteBytes(len);
}

template<class TThriftClient>
void MyThriftClient<TThriftClient>::ReadBytes(uint32_t len, bool input) {

  if (input)
    _cltITransport->consume(len);
  else
    _cltOTransport->consume(len);
}

template<class TThriftClient>
MyThriftClient<TThriftClient>::~MyThriftClient() {
  Disconnect();
  delete _client;
}

template<class TThriftClient>
TThriftClient *MyThriftClient<TThriftClient>::GetClient() const {
  return _client;
}

template<class TThriftClient>
bool MyThriftClient<TThriftClient>::IsConnected() {
  return _cltITransport->isOpen() && _cltOTransport->isOpen();
}

template<class TThriftClient>
void MyThriftClient<TThriftClient>::Connect() {
  if (!IsConnected()) {
    try {
      _cltITransport->open();
      _cltOTransport->open();
    } catch (TException &tx) {
      throw tx;
    }
  }
}

template<class TThriftClient>
void MyThriftClient<TThriftClient>::Disconnect() {
  if (IsConnected()) {
    try {
      _cltITransport->close();
      _cltOTransport->close();
    } catch (TException &tx) {
      throw tx;
    }
  }
}

template<class TThriftClient>
void MyThriftClient<TThriftClient>::KeepAlive() {

}

// TODO: Implement KeepAlive Timeout
template<class TThriftClient>
void MyThriftClient<TThriftClient>::KeepAlive(int timeout_ms) {

}

} // namespace social_network


#endif //SOCIAL_NETWORK_MICROSERVICES_MYTHRIFTCLIENT_H
