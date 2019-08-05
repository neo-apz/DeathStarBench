//
// Created by Arash on 2019-08-03.
//

#include "../Helpers/commons.h"

#include "MyUrlShortenHandler.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace social_network;

// stopwatch sw_deser;
// stopwatch sw_service;
// stopwatch sw_dispatch;

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
  // urls.emplace_back("https://url_1.com");
  // urls.emplace_back("https://url_2.com");
  // urls.emplace_back("https://url_0.com");
  // urls.emplace_back("https://url_1.com");
  // urls.emplace_back("https://url_2.com");
  // urls.emplace_back("https://url_0.com");
  // urls.emplace_back("https://url_1.com");
  // urls.emplace_back("https://url_2.com");
  // urls.emplace_back("https://url_0.com");
  // urls.emplace_back("https://url_1.com");
  // urls.emplace_back("https://url_2.com");

//  client.UploadUrls(returned_urls, req_id, urls);

  uint64_t count = 0;
  uint64_t iterations = 10000;

  #if defined(FLEXUS)
  while (true)
  #else
  while (iterations--)
  #endif
  {
    client.send_UploadUrls(req_id, urls);
    
    #if defined(FLEXUS)
    NOTIFY((uint64_t)(0xBAAAAAD1));
    #endif
    
    processor->process(protocol, protocol, nullptr);
    
    #if defined(FLEXUS)
    NOTIFY((uint64_t)(0xBAAAAAD0));
    #endif

    client.recv_UploadUrls(returned_urls);

    count += returned_urls.size();
    
    // for (auto &url : returned_urls) {
    //   std::cout << url << std::endl;
    // }
  }

  // std::cout << "AVG de/ser Time(us): " << sw_getAVG(sw_deser) << std::endl;

  // std::cout << "AVG service Time(us): " << sw_getAVG(sw_service) << std::endl;

  // std::cout << "AVG process Time(us): " << sw_getAVG(sw_dispatch) << std::endl;


//  std::cout << "Starting the url-shorten-service server..." << std::endl;

  return 0;
}