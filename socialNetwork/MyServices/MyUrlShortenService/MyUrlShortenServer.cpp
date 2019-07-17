#include "../Helpers/commons.h"
#include <signal.h>

#include "MyUrlShortenHandler.h"

#include <chrono>
#include <thread>

// using apache::thrift::server::TThreadedServer;
// using apache::thrift::transport::TServerSocket;
// using apache::thrift::transport::TFramedTransportFactory;
// using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace social_network;

uint64_t start, stop;
uint64_t total = 0;
uint64_t count = 0;

stopwatch sw_deser;
stopwatch sw_service;
stopwatch sw_dispatch;

void sigintHandler(int sig) {
  std::cout << "Stopping the url-shorten-service server..." << std::endl;

  std::cout << "AVG Time(us): " << (total * 1.0) / count << std::endl;


  std::cout << "AVG de/ser Time(us): " << sw_getAVG(sw_deser) << std::endl;

  std::cout << "AVG service Time(us): " << sw_getAVG(sw_service) << std::endl;

  std::cout << "AVG dispatch Time(us): " << sw_getAVG(sw_dispatch) << std::endl;

  exit(EXIT_SUCCESS);
}

class MyUrlShortenServiceCloneFactory : virtual public MyUrlShortenServiceIfFactory {
 public:
  virtual ~MyUrlShortenServiceCloneFactory() {}
  virtual MyUrlShortenServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
  {
//    stdcxx::shared_ptr<TSocket> sock = stdcxx::dynamic_pointer_cast<TSocket>(connInfo.transport);
//    std::cout << "Incoming connection\n";
//    std::cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
//    std::cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
//    std::cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
//    std::cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";

    MyUrlShortenHandler* handler = new MyUrlShortenHandler();
    start = now();
//    return new MyUrlShortenHandler;
    return handler;
  }
  virtual void releaseHandler(MyUrlShortenServiceIf* handler) {
    stop = now();
    uint64_t elapsed = stop - start;
    total += elapsed;
    count++;
    std::cout << "\tTime(us): " << elapsed << "\n";
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
//  stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TCompactProtocolFactory());

  // TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  TSimpleServer server(processorFactory, serverTransport, transportFactory, protocolFactory);
  // TThreadedServer

  std::cout << "Starting the url-shorten-service server..." << std::endl;
  server.serve();

  return 0;
}