
#include <signal.h>

#include <thrift/server/TThreadedServer.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "../utils.h"
#include "MyUniqueIdHandler.h"

#include "../MyCommon/MyClientPool.h"
#include "../MyCommon/MyThriftClient.h"

#ifdef __aarch64__
  #include "../MyCommon/MagicBreakPoint.h"
#endif


using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;
using namespace social_network;

using namespace std;

uint64_t num_iterations;
std::mutex thread_lock;
std::string machine_id;

#ifdef __aarch64__
  cpu_set_t cpuSet[16];
#endif

#ifdef FLEXUS
  volatile bool start = false;
#endif

#define ITERATION 10000
#define BUFFER_SIZE  50

// extern "C" {
// #include "../MyCommon/rd_pgmap.h"
// };


void ClientSendUniqueIdPointerBased(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                                    uint64_t buffer_size){
  
  uint8_t *cltIBufPtr, *cltOBufPtr;
  uint32_t ISz, OSz, len;

  uniqueIdClient->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // for (int i = 0; i < buffer_size; i++){
  //   read_pagemap( (unsigned long) (cltIBufPtr + i));
  // }

  std::cout << "Before Send: IBuf:" << (uint64_t) cltIBufPtr
            << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
            << " OSz: " << OSz << std::endl;

  ISz = buffer_size - ISz;
  OSz = buffer_size - OSz;

#ifdef __aarch64__
  len = call_magic_4_64(1234, (uint64_t) cltIBufPtr, ISz, (uint64_t) cltOBufPtr, OSz);
#else
  MyThriftClient<MyUniqueIdServiceClient> newUniqueIdClient(cltIBufPtr, ISz, cltOBufPtr, OSz);
  
  newUniqueIdClient.Connect();
  auto client = newUniqueIdClient.GetClient();

  int64_t req_id = 0xFFFFFFFFFFFF; // rand!
  PostType::type post_type = (PostType::type) 0;

  client->send_UploadUniqueId(req_id, post_type);

  newUniqueIdClient.GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  std::cout << "After Send: IBuf:" << (uint64_t) cltIBufPtr
            << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
            << " OSz: " << OSz << std::endl;

  len = OSz;
#endif

  uniqueIdClient->WroteBytes(len, false);
}

void ClientRecvUniqueIdPointerBased(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                                    uint64_t buffer_size){
  
  uint8_t *cltIBufPtr, *cltOBufPtr;
  uint32_t ISz, OSz, len;

  uniqueIdClient->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  std::cout << "Before Receive: IBuf:" << (uint64_t) cltIBufPtr
            << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
            << " OSz: " << OSz << std::endl;

  // ISz = buffer_size - ISz;
  // OSz = buffer_size - OSz;

  MyThriftClient<MyUniqueIdServiceClient> newUniqueIdClient(cltIBufPtr, buffer_size - ISz,
                                                            cltOBufPtr, buffer_size - OSz);
  
  newUniqueIdClient.Connect();
  newUniqueIdClient.WroteBytes(ISz, true);
  
  auto client = newUniqueIdClient.GetClient();

  client->recv_UploadUniqueId();

  newUniqueIdClient.GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  std::cout << "After Receive: IBuf:" << (uint64_t) cltIBufPtr
            << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
            << " OSz: " << OSz << std::endl;

  len = OSz;
  uniqueIdClient->ReadBytes(len, false);
}

void ClientRecvUniqueId(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  client->recv_UploadUniqueId();
}

void GenAndProcessUniqueIdReqs(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                             std::shared_ptr<MyUniqueIdHandler> handler, uint64_t buffer_size,
                             int tid, int max_tid){
  
  auto srvIProt = uniqueIdClient->GetClient()->getOutputProtocol();
  auto srvOProt = uniqueIdClient->GetClient()->getInputProtocol();

  std::shared_ptr<MyUniqueIdServiceProcessor> processor =
      std::make_shared<MyUniqueIdServiceProcessor>(handler);

  uint64_t count = num_iterations;

  #ifdef FLEXUS
  if (tid == max_tid) {
    start = true;
    BREAKPOINT();
  }

  while(!start);
  #endif

  while (count--){
    ClientSendUniqueIdPointerBased(uniqueIdClient, buffer_size);

    std::cout << "Processing Thread " << tid << " count=" << count+1  << std::endl;
    processor->process(srvIProt, srvOProt, nullptr);
    
    #ifdef __aarch64__
      PROCESS_END();
    #endif

    ClientRecvUniqueIdPointerBased(uniqueIdClient, buffer_size);
  }
}

int main(int argc, char *argv[]) {
  init_logger();

  uint64_t num_threads;

  if (argc != 3) {
    cout << "Invalid input! Usage: ./MyUniqueIdService <num_threads> <iterations> \n" << endl;
    exit(-1);
  } else {
    num_threads = atoi(argv[1]);
    num_iterations = atoi(argv[2]);
  }

  #ifdef __aarch64__
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
  #endif

  uint64_t buffer_size = num_iterations * BUFFER_SIZE;
  std::cout << "Buffer size: " << buffer_size << std::endl;

  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  MyThriftClient<MyUniqueIdServiceClient>* uniqueIdClients[num_threads];

  MyClientPool<MyThriftClient<MyComposePostServiceClient>> composeClientPool (
    "compose-post", buffer_size, 16, 16, 1000);

  std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
                                              &thread_lock, machine_id,
                                              &composeClientPool);
  
  std::thread threads[num_threads];

  for (int i = 0; i < num_threads; i++) {
    uniqueIdClients[i] = new MyThriftClient<MyUniqueIdServiceClient>(buffer_size);

    threads[i] = std::thread(GenAndProcessUniqueIdReqs,
                              uniqueIdClients[i], handler, buffer_size,
                              i, num_threads - 1);

    #ifdef __aarch64__
      CPU_ZERO(&cpuSet[i]);
      CPU_SET(i+1, &cpuSet[i]);
      pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuSet[i]);
    #endif
  }

  for (int i = 0; i < num_threads; i++) {
    threads[i].join();
  }

  for (int i = 0; i < num_threads; i++) {
    delete uniqueIdClients[i];
  }

  return 0;
}
