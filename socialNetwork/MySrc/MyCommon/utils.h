#ifndef SOCIAL_NETWORK_MICROSERVICES_UTILS_H
#define SOCIAL_NETWORK_MICROSERVICES_UTILS_H

#include <string>
#include <fstream>
#include <iostream>

#include "logger.h"
#include "RandomGenerator.h"

#ifdef CEREBROS
#include "CerebrosProcessor.h"
#else
#include "MyThriftClient.h"
#include "NebulaThriftProcessor.h"
#endif

using namespace my_social_network;
using namespace std;

#define NUM_TEMPLATE_CLIENTS     20
#define NUM_MSGS_PER_CLIENT      1

uint64_t num_threads, num_nodes, num_iterations;

volatile bool start = false;
pthread_barrier_t barrier;

template<typename TThriftHandler>
using init_pools_function = void (*)(RandomGenerator*, rpcNUMAContext* ctx, std::shared_ptr<TThriftHandler>, int tid); // type for conciseness

// typedef void (*init_pools_function)(RandomGenerator*, rpcNUMAContext* ctx, int tid); // type for conciseness

template <class TThriftClient>
#ifdef CEREBROS
void GenRequests(TThriftClient** clients, RandomGenerator *randGen) {

  for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new TThriftClient(randGen);
  }
}
#else
void GenRequests(MyThriftClient<TThriftClient> **clients, RandomGenerator *randGen){

	uint64_t buffer_size = NUM_MSGS_PER_CLIENT * BASE_BUFFER_SIZE;
	
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<TThriftClient>(buffer_size);

		auto client = clients[i]->GetClient();
		for (int i = 0; i < NUM_MSGS_PER_CLIENT; i++) {
			client->initArgs(randGen);
			client->send_RandReq(randGen);
		}
	}

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;  
}
#endif

template <class TThriftClient, class TThriftHandler, class TTProcessor>
void GenAndProcessReqs(rpcNUMAContext* ctx,
											 int tid,
											 std::shared_ptr<TThriftHandler> handler,
											 init_pools_function<TThriftHandler> initFunc) {

  // LOG(warning) << "User TID: " << tid << " TID: " << std::this_thread::get_id();
  RandomGenerator randGen(tid);

	// Generate fake requests
#ifdef CEREBROS
	TThriftClient *clients[NUM_TEMPLATE_CLIENTS];
#else
	MyThriftClient<TThriftClient>* clients[NUM_TEMPLATE_CLIENTS];
#endif

	GenRequests<TThriftClient>(clients, &randGen);
	std::shared_ptr<TTProcessor> proc = std::make_shared<TTProcessor>(handler);

#ifdef CEREBROS
	auto processor = new CerebrosProcessor<TTProcessor, TThriftClient>(ctx, tid, proc, clients);
#else
	auto processor = new NebulaThriftProcessor<TTProcessor, TThriftClient>(ctx, tid, proc, clients);
#endif

	initFunc(&randGen, ctx, handler, tid);

  pthread_barrier_wait(&barrier);

  if (tid == 0) {
		fprintf(stdout,"Init done! Ready to start execution!\n");
    ctx->readyForTiming();

    start = true;
    // LOG(warning) << "Process Phase Started!!";
  }

  while(!start);

  uint64_t count = 1;

  while (count <= num_iterations) {
    processor->process(count++);
  }

  #ifdef SW
    processor->printSWResults();
  #endif

  // if (tid == max_tid)
  //   LOG(warning) << "Process Phase finished!";
}

rpcNUMAContext* MainInit(int argc, char *argv[]) {
	init_logger();

  if (argc != 4) {
    cout << "Invalid input! Usage: " << argv[0] << " <total # nodes> <num_threads> <iterations> \n" << endl;
    exit(-1);
  } else {
    num_nodes = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    num_iterations = atoi(argv[3]);
  }

  pthread_barrier_init(&barrier, NULL, num_threads);

  int node_id = 1;
  // Create the RPC NUMA Context!
  rpcNUMAContext* rpcContext = new rpcNUMAContext(node_id, num_nodes, num_threads+1);

	return rpcContext;
}

template <class TThriftClient, class TThriftHandler, class TTProcessor>
int MainProcess(
	rpcNUMAContext* rpcContext,
	std::shared_ptr<TThriftHandler> handler,
	init_pools_function<TThriftHandler> f) {
	
	cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(1, &mask); // pin the main thread to core #1
  sched_setaffinity(0, sizeof(mask), &mask);

	
	std::thread processThreads[num_threads+1];

  int coreID = 0;
  while (coreID <= num_threads) {

    if (coreID == 1) { //skip this core, this will only run the main thread
      coreID++;
      continue;
    }

    processThreads[coreID] = std::thread(
					GenAndProcessReqs<TThriftClient, TThriftHandler, TTProcessor>,
					rpcContext,
					coreID,
					handler,
					f);

    CPU_ZERO(&mask);
    CPU_SET(coreID, &mask);
    int error = pthread_setaffinity_np(processThreads[coreID].native_handle(), sizeof(cpu_set_t), &mask);
    if (error) {
      printf("Could not bind thread %d to core %d! (error %d)\n", coreID, coreID, error);
			return 1;
    }
    coreID++;
  }

	coreID = 0;
  while (coreID <= num_threads) {
    if (coreID == 1) { //skip this core, this will only run the main thread
      coreID++;
      continue;
    }

    processThreads[coreID].join();

    coreID++;
  }

  return 0;
}

#endif //SOCIAL_NETWORK_MICROSERVICES_UTILS_H
