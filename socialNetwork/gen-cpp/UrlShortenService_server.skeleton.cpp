// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "UrlShortenService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::social_network;

class UrlShortenServiceHandler : virtual public UrlShortenServiceIf {
 public:
  UrlShortenServiceHandler() {
    // Your initialization goes here
  }

  void UploadUrls(std::vector<std::string> & _return, const int64_t req_id, const std::vector<std::string> & urls) {
    // Your implementation goes here
    printf("UploadUrls\n");
  }

  void GetExtendedUrls(std::vector<std::string> & _return, const int64_t req_id, const std::vector<std::string> & shortened_urls) {
    // Your implementation goes here
    printf("GetExtendedUrls\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  ::apache::thrift::stdcxx::shared_ptr<UrlShortenServiceHandler> handler(new UrlShortenServiceHandler());
  ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new UrlShortenServiceProcessor(handler));
  ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

