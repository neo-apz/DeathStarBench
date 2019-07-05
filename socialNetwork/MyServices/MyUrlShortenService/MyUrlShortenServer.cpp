#include <thrift/protocol/TBinaryProtocol.h>
// #include <thrift/server/TThreadedServer.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/stdcxx.h>

#include <signal.h>

// #include "../utils.h"
#include <string>
#include <fstream>
#include <iostream>

#include "MyUrlShortenHandler.h"

// using apache::thrift::server::TThreadedServer;
// using apache::thrift::transport::TServerSocket;
// using apache::thrift::transport::TFramedTransportFactory;
// using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace social_network;

void sigintHandler(int sig) {
  std::cout << "Stopping the url-shorten-service server..." << std::endl;
  exit(EXIT_SUCCESS);
}

class MyUrlShortenServiceCloneFactory : virtual public MyUrlShortenServiceIfFactory {
 public:
  virtual ~MyUrlShortenServiceCloneFactory() {}
  virtual MyUrlShortenServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
  {
    stdcxx::shared_ptr<TSocket> sock = stdcxx::dynamic_pointer_cast<TSocket>(connInfo.transport);
    std::cout << "Incoming connection\n";
    std::cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
    std::cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
    std::cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
    std::cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
    return new MyUrlShortenHandler;
  }
  virtual void releaseHandler(MyUrlShortenServiceIf* handler) {
    delete handler;
  }
};


int main(int argc, char *argv[]) {
  signal(SIGINT, sigintHandler);

  int port = 9090;

  // stdcxx::shared_ptr<MyUrlShortenHandler> handler(new MyUrlShortenHandler());
  // stdcxx::shared_ptr<TProcessor> processor(new MyUrlShortenServiceProcessor(handler));
  stdcxx::shared_ptr<MyUrlShortenServiceIfFactory> cloneFactory (new MyUrlShortenServiceCloneFactory());
  stdcxx::shared_ptr<TProcessorFactory> processorFactory (new MyUrlShortenServiceProcessorFactory(cloneFactory));

  stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket("localhost", port));
  stdcxx::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
  stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  // TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  TSimpleServer server(processorFactory, serverTransport, transportFactory, protocolFactory);
  // TThreadedServer

  std::cout << "Starting the url-shorten-service server..." << std::endl;
  server.serve();

  return 0;
}