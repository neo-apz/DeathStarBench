//
// Created by Arash on 2019-08-03.
//

#include "../Helpers/commons.h"

#include "MyUrlShortenHandler.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace social_network;

stopwatch sw_deser;
stopwatch sw_service;
stopwatch sw_dispatch;

int main(int argc, char *argv[]) {
//  signal(SIGINT, sigintHandler);

  stdcxx::shared_ptr<MyUrlShortenHandler> handler(new MyUrlShortenHandler());

//  MyUrlShortenServiceProcessor processor(handler);

  stdcxx::shared_ptr<MyUrlShortenServiceProcessor> processor(new MyUrlShortenServiceProcessor(handler));
  stdcxx::shared_ptr<TTransport> transport(new TMemoryBuffer(4096));
  stdcxx::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));



  MyUrlShortenServiceClient client(protocol);
  int64_t req_id = 1200; // rand!

  std::vector<std::string> urls;
  std::vector<std::string> returned_urls;

  urls.emplace_back("https://url_0.com");
  urls.emplace_back("https://url_1.com");
  urls.emplace_back("https://url_2.com");

//  client.UploadUrls(returned_urls, req_id, urls);
  client.send_UploadUrls(req_id, urls);

  processor->process(protocol, protocol, nullptr);

  client.recv_UploadUrls(returned_urls);

  for (auto &url : returned_urls) {
    std::cout << url << std::endl;
  }



//  std::cout << "Starting the url-shorten-service server..." << std::endl;

  return 0;
}