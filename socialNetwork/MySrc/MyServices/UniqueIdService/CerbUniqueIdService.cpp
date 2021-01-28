#include <CerebrosProcessor.h>
#include <RandomGenerator.h>

#include "UniqueIdHandler.h"

#include <thread>
#include <cer_rpc.h>

using namespace my_social_network;

using namespace std;

uint64_t num_nodes;
std::mutex thread_lock;
std::string machine_id;

volatile bool start = false;
pthread_barrier_t   barrier;

#define NUM_TEMPLATE_MSGS  20

#define WARM_UP_ITER  100

#define CEREBROS	1

void GenRequests(UniqueIdServiceClient** clients,
								 RandomGenerator *randGen) {

  for (int i = 0; i < NUM_TEMPLATE_MSGS; i++) {
		clients[i]->uploadUniqueId_args = new UniqueIdService_UploadUniqueId_args(randGen);
  }
}

void GenAndProcessReqs(rpcNUMAContext* ctx,
											 int tid,
											 std::shared_ptr<UniqueIdHandler> handler){

	RandomGenerator randGen(tid);

	// Generate fake requests
	UniqueIdServiceClient *clients[NUM_TEMPLATE_MSGS];
  GenRequests(clients, &randGen);
	
	std::shared_ptr<UniqueIdServiceCerebrosProcessor> proc = 
		std::make_shared<UniqueIdServiceCerebrosProcessor>(handler);

	auto processor = new CerebrosProcessor<UniqueIdServiceCerebrosProcessor, UniqueIdServiceClient>(ctx, tid, proc, clients);

  // Wait for all the threads to reach here & then pause.
  pthread_barrier_wait(&barrier);

  if(tid == 0) {
    fprintf(stdout,"Init done! Ready to start execution!\n");
    ctx->readyForTiming();

		start = true;
  }

	while(!start);

	uint64_t count = 0;

  // Process loop
  while (true) {
		processor->process(count);
    count++;
  }

}

int main(int argc, char *argv[]) {

  uint64_t num_threads;

  if (argc != 3) {
    cout << "Invalid input! Usage: " << argv[0]  << " <total # nodes> <num_threads> \n" << endl;
    exit(-1);
  } else {
    num_nodes = atoi(argv[1]);
    num_threads = atoi(argv[2]);
  }

  cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(1, &mask); // pin the main thread to core #1
  sched_setaffinity(0, sizeof(mask), &mask);

  pthread_barrier_init(&barrier, NULL, num_threads);

  int node_id = 1;
  // Create the RPC NUMA Context!
  rpcNUMAContext* rpcContext = new rpcNUMAContext(node_id, num_nodes, num_threads+1);

	if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  std::shared_ptr<UniqueIdHandler> handler = std::make_shared<UniqueIdHandler>(
																								&thread_lock, machine_id, nullptr);

  std::thread processThreads[num_threads+1];

  int coreID = 0;
  while (coreID <= num_threads) {

    if (coreID == 1) { //skip this core, this will only run the main thread and handle interrupts
      coreID++;
      continue;
    }

    processThreads[coreID] = std::thread(GenAndProcessReqs,
																					rpcContext,
																					coreID,
																					handler);

    CPU_ZERO(&mask);
    CPU_SET(coreID, &mask);
    int error = pthread_setaffinity_np(processThreads[coreID].native_handle(), sizeof(cpu_set_t), &mask);
    if (error) {
      printf("Could not bind thread %d to core %d! (error %d)\n", coreID, coreID, error);
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
