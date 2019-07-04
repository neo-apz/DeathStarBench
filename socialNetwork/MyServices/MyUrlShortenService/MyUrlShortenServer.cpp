#include <thrift/protocol/TBinaryProtocol.h>
// #include <thrift/server/TThreadedServer.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
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


int main(int argc, char *argv[]) {
  signal(SIGINT, sigintHandler);

  int port = 9090;

  ::apache::thrift::stdcxx::shared_ptr<MyUrlShortenHandler> handler(new MyUrlShortenHandler());
  ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new MyUrlShortenServiceProcessor(handler));
  ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket("localhost", port));
  ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
  ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  // TThreadedServer

  std::cout << "Starting the url-shorten-service server..." << std::endl;
  server.serve();

  return 0;
}