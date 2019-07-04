#include <thrift/protocol/TBinaryProtocol.h>
// #include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

// #include "../utils.h"
#include <string>
#include <fstream>
#include <iostream>

#include "../gen-cpp/MyUrlShortenService.h"


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace social_network;

// void sigintHandler(int sig) {
//   std::cout << "Stopping the url-shorten-service server..." << std::endl;
//   exit(EXIT_SUCCESS);
// }


int main(int argc, char *argv[]) {
//   signal(SIGINT, sigintHandler);

  int port = 9090;

  ::apache::thrift::stdcxx::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  ::apache::thrift::stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
  ::apache::thrift::stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  MyUrlShortenServiceClient client(protocol);

  try
  {
    transport->open();
    int64_t req_id = 1200; // rand!

    std::vector<std::string> urls;
    std::vector<std::string> returned_urls;

    urls.emplace_back("https://url_0.com");
    urls.emplace_back("https://url_1.com");
    urls.emplace_back("https://url_2.com");
      
    std::cout << "Sending a req to the url-shorten-service server..." << std::endl;
      
    client.UploadUrls(returned_urls, req_id, urls);

    transport->close();
  }
  catch(const TException& tx)
  {
      std::cout << "ERROR: " << tx.what() << std::endl;
      return 1;
  }

  std::cout << "Stopping the client..." << std::endl;
  return 0;
}