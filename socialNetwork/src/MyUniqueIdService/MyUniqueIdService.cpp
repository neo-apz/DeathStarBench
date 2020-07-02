
#include <signal.h>

#include <thrift/server/TThreadedServer.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "../utils.h"
#include "MyUniqueIdHandler.h"

#include "../MyCommon/MyThriftClient.h"
#include "../MyCommon/MagicBreakPoint.h"

using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;
using namespace social_network;

#define BUFFER_SIZE (1024 * 32)
#define ITERATION 1

void sigintHandler(int sig) {
  exit(EXIT_SUCCESS);
}

void ClientSendUniqueIdPointerBased(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                      uint32_t count){
  
  uint8_t *cltIBufPtr, *cltOBufPtr;
  uint32_t ISz, OSz, len;

  uniqueIdClient->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  ISz = BUFFER_SIZE - ISz;
  OSz = BUFFER_SIZE - OSz;

  std::cout << "After GetBuffer: IBuf:" << (uint64_t) cltIBufPtr
            << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
            << " OSz: " << OSz << std::endl;

 MyThriftClient<MyUniqueIdServiceClient> newUniqueIdClient(cltIBufPtr, ISz, cltOBufPtr, OSz);
  
  // newUniqueIdClient.Connect();
  auto client = newUniqueIdClient.GetClient();

  int64_t req_id = 0xFFFFFFFFFFFF; // rand!
  PostType::type post_type = (PostType::type) 0;

  while(count--){
    client->send_UploadUniqueId(req_id, post_type);
  }

  newUniqueIdClient.GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  std::cout << "After NewClient: IBuf:" << (uint64_t) cltIBufPtr
            << " ISz: " << ISz << " OBuf: " << (uint64_t) cltOBufPtr
            << " OSz: " << OSz << std::endl;

  len = OSz - BUFFER_SIZE;
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

void ProcessUniqueIdRequests(std::shared_ptr<MyUniqueIdServiceProcessor> processor,
                     MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                     uint32_t count){
  
  auto srvIProt = uniqueIdClient->GetClient()->getOutputProtocol();
  auto srvOProt = uniqueIdClient->GetClient()->getInputProtocol();

  std::cout << "Before the process loop." << std::endl;

  while (count--){
    processor->process(srvIProt, srvOProt, nullptr);
  }
}

void ClientRecvUniqueId(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient,
                      uint32_t count){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  int64_t req_id = 0xFFFFFFFFFFFF; // rand!
  PostType::type post_type = (PostType::type) 0;

  while(count--){
    client->recv_UploadUniqueId();
  }
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigintHandler);
  init_logger();

  std::string machine_id;
  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  std::mutex thread_lock;

  MyThriftClient<MyUniqueIdServiceClient> uniqueIdClient(BUFFER_SIZE);
  MyThriftClient<MyComposePostServiceClient> composeClient(BUFFER_SIZE);

  std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
    &thread_lock, machine_id, &composeClient);

  std::shared_ptr<MyUniqueIdServiceProcessor> processor = 
    std::make_shared<MyUniqueIdServiceProcessor>(handler);

  std::cout << "Generating requests ..." << std::endl;
  // ClientSendUniqueId(&uniqueIdClient, 3);
  ClientSendUniqueIdPointerBased(&uniqueIdClient, ITERATION);

  // std::cout << "Pointer: " << (&uniqueIdClient) << std::endl;
  // qflex_magic_break(1234, reinterpret_cast<uint64_t>(&uniqueIdClient), 3);

  std::cout << "Processing the generated requests ..." << std::endl;
  ProcessUniqueIdRequests(processor, &uniqueIdClient, ITERATION);

  std::cout << "Getting responses ..." << std::endl;
  ClientRecvUniqueId(&uniqueIdClient, ITERATION);

  return 0;
}
