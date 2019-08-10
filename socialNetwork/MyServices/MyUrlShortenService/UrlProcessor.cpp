//
// Created by Arash on 2019-08-03.
//

#include "../Helpers/commons.h"

#include "MyUrlShortenHandler.h"

#include "../mythrift/MyTMemoryBuffer.h"

//#import<thread>


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace social_network;
using namespace mythrift;

//#define ITERATIONS 1000

// stopwatch sw_deser;
// stopwatch sw_service;
// stopwatch sw_dispatch;

void serverUploadUrls(uint64_t iterations, MyTMemoryBuffer *memBuffer) {

  stdcxx::shared_ptr<MyUrlShortenHandler> handler(new MyUrlShortenHandler());
  stdcxx::shared_ptr<MyUrlShortenServiceProcessor> processor(new MyUrlShortenServiceProcessor(handler));

#if defined(FLEXUS)
  while (true)
#else
  while (iterations--)
#endif
  {

#if defined(FLEXUS)
    NOTIFY((PROCESS_BEGIN);
#endif

    processor->process(memBuffer->_srvIProt, memBuffer->_srvOProt, nullptr);

#if defined(FLEXUS)
    NOTIFY((PROCESS_END);
#endif

  }

}

void clientSendUploadUrls(uint64_t iterations, MyTMemoryBuffer *memBuffer){

  MyUrlShortenServiceClient client(memBuffer->_srvOProt, memBuffer->_srvIProt);

  int64_t req_id = 1200000; // rand!

  std::vector<std::string> urls;
  std::vector<std::string> returned_urls;

  urls.emplace_back("https://url_0.com");

  uint64_t count = 0;

#if defined(FLEXUS)
  while (true)
#else
  while (iterations--)
#endif
  {

    client.send_UploadUrls(req_id, urls);

//#if defined(FLEXUS)
//    NOTIFY((uint64_t)(0xBAAAAAD1));
//#endif


//#if defined(FLEXUS)
//    NOTIFY((uint64_t)(0xBAAAAAD0));
//#endif

  }

}

void clientRecvUploadUrls(uint64_t iterations, MyTMemoryBuffer *memBuffer){

  MyUrlShortenServiceClient client(memBuffer->_srvOProt, memBuffer->_srvIProt);

  std::vector<std::string> returned_urls;

  uint64_t count = 0;

  while (iterations--) {



    client.recv_UploadUrls(returned_urls);
//
    count += returned_urls.size();

//    for (auto &url : returned_urls) {
//      std::cout << url << std::endl;
//    }

  }

  std::cout << "count = " << count << std::endl;

}

int main(int argc, char *argv[]) {

//  stdcxx::shared_ptr<MyUrlShortenHandler> handler(new MyUrlShortenHandler());
//
////  MyUrlShortenServiceProcessor processor(handler);
//
//  stdcxx::shared_ptr<MyUrlShortenServiceProcessor> processor(new MyUrlShortenServiceProcessor(handler));
//  stdcxx::shared_ptr<TMemoryBuffer> transport(new TMemoryBuffer(4096));
//  stdcxx::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));

  uint64_t iterations;

  if (argc != 2) {
    cout << "Invalid input! Usage: ./UrlProcessor <iterations> \n" << endl;
    exit(-1);
  } else {
    iterations = atoi(argv[1]);
  }

  MyTMemoryBuffer memBuffer(iterations * 100);

  std::thread clientSendThread(clientSendUploadUrls, iterations, &memBuffer);

  clientSendThread.join();

  std::thread serverThread(serverUploadUrls, iterations, &memBuffer);

  serverThread.join();

  std::thread clientRecvThread(clientRecvUploadUrls, iterations, &memBuffer);

  clientRecvThread.join();

  // std::cout << "AVG de/ser Time(us): " << sw_getAVG(sw_deser) << std::endl;

  // std::cout << "AVG service Time(us): " << sw_getAVG(sw_service) << std::endl;

  // std::cout << "AVG process Time(us): " << sw_getAVG(sw_dispatch) << std::endl;


//  std::cout << "Starting the url-shorten-service server..." << std::endl;

  return 0;
}