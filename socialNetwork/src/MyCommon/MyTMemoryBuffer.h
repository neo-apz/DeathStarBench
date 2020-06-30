
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

//    std::shared_ptr<TMemoryBuffer> srvITransport(new TMemoryBuffer(sz));
//    this->_srvITransport = srvITransport;
//    std::shared_ptr<TMemoryBuffer> srvOTransport(new TMemoryBuffer(sz));
//    this->_srvOTransport = srvOTransport;

    // std::cout << "_srvITransport: " << this->_srvITransport.get() << std::endl;
    // std::cout << "_srvOTransport: " << this->_srvOTransport.get() << std::endl;
    // std::cout.flush();

    this->_srvITransport = std::make_shared<TMemoryBuffer>(sz);
    this->_srvOTransport = std::make_shared<TMemoryBuffer>(sz);


//    std::shared_ptr<TProtocol> srvIProt(new TCompactProtocol(_srvITransport));
//    _srvIProt = srvIProt;
//    std::shared_ptr<TProtocol> srvOProt(new TCompactProtocol(_srvOTransport));
//    _srvOProt = srvOProt;

    // std::cout << "_srvIProt: " << this->_srvIProt.get() << std::endl;
    // std::cout << "_srvOProt: " << this->_srvOProt.get() << std::endl;
    // std::cout.flush();

    this->_srvIProt = std::make_shared<TCompactProtocol>(this->_srvITransport);
    this->_srvOProt = std::make_shared<TCompactProtocol>(this->_srvOTransport);
  }

//  bool clientFlags = false;
//  bool serverFlags = false;

  std::shared_ptr<TMemoryBuffer> _srvITransport;
  std::shared_ptr<TMemoryBuffer> _srvOTransport;

  std::shared_ptr<TCompactProtocol> _srvIProt;
  std::shared_ptr<TCompactProtocol> _srvOProt;

};

}

#endif //SOCCIALNET_MYSERVICES_MYTMEMORYBUFFER_H
