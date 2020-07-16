/*
 * 64-bit Unique Id Generator
 *
 * ------------------------------------------------------------------------
 * |0| 11 bit machine ID |      40-bit timestamp         | 12-bit counter |
 * ------------------------------------------------------------------------
 *
 * 11-bit machine Id code by hasing the MAC address
 * 40-bit UNIX timestamp in millisecond precision with custom epoch
 * 12 bit counter which increases monotonically on single process
 *
 */

#include <signal.h>

#include <thrift/server/TSimpleServer.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "UniqueIdHandler.h"

#include <pthread.h>


// using apache::thrift::server::TThreadedServer;
using apache::thrift::server::TSimpleServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TCompactProtocolFactory;
using namespace social_network;

stopwatch sw;

uint64_t count = 0;

Stopwatch<std::chrono::microseconds> SW;

void sigintHandler(int sig) {
  // sw_stop(&sw);
  // SW.stop();
  // SW.post_process();

  // std::cout << "AVG Time: " << SW.mean() * 1.0 << std::endl;
  // std::cout << "STDEV: " << SW.standard_deviation() << std::endl;

  // std::cout << "COUNT " << count << std::endl;


  // std::cout << "AVG Time: " << sw_getAVG(sw) << " us." << std::endl;
  // std::cout << "Total " << sw.total << std::endl;
  // std::cout << "Count " << sw.count << std::endl;
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigintHandler);
  init_logger();
  // SetUpTracer("config/jaeger-config.yml", "unique-id-service");

  // json config_json;
  // if (load_config_file("config/service-config.json", &config_json) != 0) {
  //   exit(EXIT_FAILURE);
  // }

  int port = 4000;

  std::string compose_post_addr;

  if (argc != 2) {
    std::cout << "Invalid input! Usage: ./binary <address> \n" << std::endl;
    exit(-1);
  } else {
    compose_post_addr = argv[1];
    // #ifdef __aarch64__
    //   SET_ITERATION_COUNT(num_iterations);
    // #endif
  }

  int compose_post_port = 5000;

  std::string machine_id;
  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  std::mutex thread_lock;
  ClientPool<ThriftClient<ComposePostServiceClient>> compose_post_client_pool(
      "compose-post", compose_post_addr, compose_post_port, 0, 128, 1000);

  
  cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(4, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  // TSimpleServer
  TSimpleServer server (
      std::make_shared<UniqueIdServiceProcessor>(
          std::make_shared<UniqueIdHandler>(
              &thread_lock, machine_id, &compose_post_client_pool, &SW)),
      std::make_shared<TServerSocket>("0.0.0.0", port),
      std::make_shared<TFramedTransportFactory>(),
      std::make_shared<TCompactProtocolFactory>()
  );

  std::cout << "Starting the unique-id-service server ..." << std::endl;
  
  // sw_start(&sw);
  
  // SW.start();
  
  server.serve();
}
