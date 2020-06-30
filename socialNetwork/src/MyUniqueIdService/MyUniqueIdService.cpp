
#include <signal.h>

#include <thrift/server/TThreadedServer.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "../utils.h"
#include "MyUniqueIdHandler.h"

#include "../MyCommon/MyThriftClient.h"

using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;
using namespace social_network;

void sigintHandler(int sig) {
  exit(EXIT_SUCCESS);
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

  MyThriftClient<MyUniqueIdServiceClient> uniqueIdClient(1024 * 32);
  MyThriftClient<MyComposePostServiceClient> composeClient(1024 * 32);

  std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
    &thread_lock, machine_id, &composeClient);

  std::shared_ptr<MyUniqueIdServiceProcessor> processor = 
    std::make_shared<MyUniqueIdServiceProcessor>(handler);

  std::cout << "Generating requests ..." << std::endl;
  ClientSendUniqueId(&uniqueIdClient, 3);

  std::cout << "Processing the generated requests ..." << std::endl;
  ProcessUniqueIdRequests(processor, &uniqueIdClient, 3);

  std::cout << "Getting responses ..." << std::endl;
  ClientRecvUniqueId(&uniqueIdClient, 3);

  return 0;
}
