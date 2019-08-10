//
// Created by Arash on 2019-08-08.
//

#ifndef SOCCIALNET_MYSERVICES_MYTMEMORYBUFFER_H
#define SOCCIALNET_MYSERVICES_MYTMEMORYBUFFER_H

# include<thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::protocol;

namespace mythrift {

class MyTMemoryBuffer {
 public:

  MyTMemoryBuffer(uint32_t sz) {

    stdcxx::shared_ptr<TMemoryBuffer> srvITransport(new TMemoryBuffer(sz));
    _srvITransport = srvITransport;
    stdcxx::shared_ptr<TMemoryBuffer> srvOTransport(new TMemoryBuffer(sz));
    _srvOTransport = srvOTransport;

//    srvITransport = stdcxx::make_shared<TMemoryBuffer>(new TMemoryBuffer(sz));
//    srvOTransport = stdcxx::make_shared<TMemoryBuffer>(new TMemoryBuffer(sz));


    stdcxx::shared_ptr<TProtocol> srvIProt(new TCompactProtocol(srvITransport));
    _srvIProt = srvIProt;
    stdcxx::shared_ptr<TProtocol> srvOProt(new TCompactProtocol(srvOTransport));
    _srvOProt = srvOProt;

//    srvIProt = stdcxx::make_shared<TProtocol>(new TCompactProtocol(srvITransport));
//    srvOProt = stdcxx::make_shared<TProtocol>(new TCompactProtocol(srvOTransport));
  }

//  bool clientFlags = false;
//  bool serverFlags = false;

  stdcxx::shared_ptr<TMemoryBuffer> _srvITransport;
  stdcxx::shared_ptr<TMemoryBuffer> _srvOTransport;

  stdcxx::shared_ptr<TProtocol> _srvIProt;
  stdcxx::shared_ptr<TProtocol> _srvOProt;

};

}

#endif //SOCCIALNET_MYSERVICES_MYTMEMORYBUFFER_H
