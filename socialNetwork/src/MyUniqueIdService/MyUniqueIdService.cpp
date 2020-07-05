
#include <signal.h>

#include <thrift/server/TThreadedServer.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "../utils.h"
#include "MyUniqueIdHandler.h"

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
  volatile bool start = false;
#endif

#define ITERATION 10000
#define BUFFER_SIZE  50

void ClientSendUniqueIdPointerBased(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient){
  
  uint8_t *cltIBufPtr, *cltOBufPtr;
  uint32_t ISz, OSz, len;

  uniqueIdClient->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);
  ISz = BUFFER_SIZE - ISz;
  OSz = BUFFER_SIZE - OSz;
  // std::cout << "After GetBuffer: IBuf:" << (uint64_t) cltIBufPtr
  //           << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
  //           << " OSz: " << OSz << std::endl;


#ifdef __aarch64__
  len = call_magic_4_64(1234, (uint64_t) cltIBufPtr, ISz, (uint64_t) cltOBufPtr, OSz);
#else
  MyThriftClient<MyUniqueIdServiceClient> newUniqueIdClient(cltIBufPtr, ISz, cltOBufPtr, OSz);
  
  newUniqueIdClient.Connect();
  auto client = newUniqueIdClient.GetClient();

  int64_t req_id = 0xFFFFFFFFFFFF; // rand!
  PostType::type post_type = (PostType::type) 0;

  uint64_t count = num_iterations;

  while(count--){
    client->send_UploadUniqueId(req_id, post_type);
  }

  newUniqueIdClient.GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "After NewClient: IBuf:" << (uint64_t) cltIBufPtr
  //           << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
  //           << " OSz: " << OSz << std::endl;

  len = OSz;

#endif

  uniqueIdClient->WroteBytes(len, false);
}

void ClientSendUniqueId(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                      uint32_t count){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  int64_t req_id = 0xFFFFFFFFFFFF; // rand!
  PostType::type post_type = (PostType::type) 0;

  while(count--){
    client->send_UploadUniqueId(req_id, post_type);
  }
}

void ProcessUniqueIdRequests(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                             MyThriftClient<MyComposePostServiceClient> *composeClient){
  
  auto srvIProt = uniqueIdClient->GetClient()->getOutputProtocol();
  auto srvOProt = uniqueIdClient->GetClient()->getInputProtocol();

  std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
                                                  &thread_lock, machine_id,
                                                  composeClient);

  std::shared_ptr<MyUniqueIdServiceProcessor> processor =
      std::make_shared<MyUniqueIdServiceProcessor>(handler);

  uint64_t count = num_iterations;

  // std::cout << "Before the process loop." << std::endl;
  #ifdef __aarch64__
  BREAKPOINT();
  #endif

  while (count--){
    processor->process(srvIProt, srvOProt, nullptr);
  }
}

void ClientRecvUniqueId(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  uint64_t count = num_iterations;

  while(count--){
    client->recv_UploadUniqueId();
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

  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  MyThriftClient<MyUniqueIdServiceClient>* uniqueIdClients[num_threads];
  MyThriftClient<MyComposePostServiceClient>* composeClients[num_threads];
  
  std::thread clientThreads[num_threads];
  std::thread serverThreads[num_threads];

  #ifdef __aarch64__
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
  #endif

  for (int i = 0; i < num_threads; i++) {
    uniqueIdClients[i] = new MyThriftClient<MyUniqueIdServiceClient>(num_iterations * BUFFER_SIZE);
    composeClients[i] = new MyThriftClient<MyComposePostServiceClient>(num_iterations * BUFFER_SIZE);
    
    cout << "Generating requests - Thread " << i << " ... " << endl;
    clientThreads[i] = std::thread(ClientSendUniqueIdPointerBased, uniqueIdClients[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    clientThreads[i].join();
    
    // std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
    //                                               &thread_lock, machine_id,
    //                                               composeClients[i]);

    // std::shared_ptr<MyUniqueIdServiceProcessor> processor = std::make_shared<MyUniqueIdServiceProcessor>(handler);

    cout << "Processing the generated requests - Thread " << i << " ... " << endl;
    serverThreads[i] = std::thread(ProcessUniqueIdRequests, uniqueIdClients[i], composeClients[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    serverThreads[i].join();

    cout << "Getting responses - Thread " << i << " ... " << endl;
    clientThreads[i] = std::thread(ClientRecvUniqueId, uniqueIdClients[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    clientThreads[i].join();
  }


  // MyThriftClient<MyUniqueIdServiceClient> uniqueIdClient(BUFFER_SIZE);
  // MyThriftClient<MyComposePostServiceClient> composeClient(BUFFER_SIZE);

  // std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
  //   &thread_lock, machine_id, &composeClient);

  // std::shared_ptr<MyUniqueIdServiceProcessor> processor = 
  //   std::make_shared<MyUniqueIdServiceProcessor>(handler);

  // std::cout << "Generating requests ..." << std::endl;
  // ClientSendUniqueId(&uniqueIdClient, 3);
  // ClientSendUniqueIdPointerBased(&uniqueIdClient, ITERATION);

  // std::cout << "Pointer: " << (&uniqueIdClient) << std::endl;
  // qflex_magic_break(1234, reinterpret_cast<uint64_t>(&uniqueIdClient), 3);

  // std::cout << "Processing the generated requests ..." << std::endl;
  // ProcessUniqueIdRequests(processor, &uniqueIdClient, ITERATION);

  // std::cout << "Getting responses ..." << std::endl;
  // ClientRecvUniqueId(&uniqueIdClient, ITERATION);

  return 0;
}
