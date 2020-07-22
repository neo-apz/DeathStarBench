#include <signal.h>
#include <string>
#include <thread>
#include <iostream>

#include <thrift/server/TThreadedServer.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "../../gen-cpp/UniqueIdService.h"
#include "../../gen-cpp/social_network_types.h"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>


using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TCompactProtocolFactory;
using apache::thrift::protocol::TCompactProtocol;
using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::transport::TFramedTransport;
using apache::thrift::transport::TSocket;
using apache::thrift::transport::TTransport;

using namespace social_network;

#define FIVE_MINS 1000*300

void sigintHandler(int sig) {
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigintHandler);
  // init_logger();
  // SetUpTracer("config/jaeger-config.yml", "unique-id-service");

  // json config_json;
  // if (load_config_file("config/service-config.json", &config_json) != 0) {
  //   exit(EXIT_FAILURE);
  // }

  int server_port = 4000;

  std::string server_addr = "127.0.0.1";



  std::shared_ptr<TSocket> socket = std::shared_ptr<TSocket>(new TSocket(server_addr, server_port));
  socket->setKeepAlive(true);
  socket->setConnTimeout(FIVE_MINS);
  socket->setRecvTimeout(FIVE_MINS);
  socket->setSendTimeout(FIVE_MINS);
  std::shared_ptr<TTransport> transport = std::shared_ptr<TTransport>(new TFramedTransport(socket));
  std::shared_ptr<TProtocol> protocol = std::shared_ptr<TProtocol>(new TCompactProtocol(transport));

  UniqueIdServiceClient *client = new UniqueIdServiceClient(protocol);

  transport->open();

  std::cout << "Starting generating requests ..." << std::endl;

  int64_t req_id = 0xFFFFFFFFFFFF; // rand!
  PostType::type post_type = (PostType::type) 0;

  int count = 0;

  while (true)
  {
      client->UploadUniqueId(req_id, post_type);
      std::cout << "Request " << ++count << std::endl;
  }

  transport->close();
  
  
}
