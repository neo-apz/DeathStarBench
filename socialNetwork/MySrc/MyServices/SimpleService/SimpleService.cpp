#include <thread>

#include <utils.h>
#ifdef CEREBROS
#include <CerebrosProcessor.h>
#else
#include <NebulaThriftProcessor.h>
#endif
#include <NebulaClientPool.h>
#include <RandomGenerator.h>

#include "SimpleHandler.h"

using namespace my_social_network;

using namespace std;

uint64_t num_iterations;
std::mutex thread_lock;
std::string machine_id;

volatile bool start = false;
pthread_barrier_t barrier;


#define BASE_BUFFER_SIZE         50
#define NUM_TEMPLATE_CLIENTS     20
#define NUM_MSGS_PER_CLIENT      1

void ClientRecvUniqueId(MyThriftClient<UniqueIdServiceClient> *uniqueIdClient){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  client->recv_UploadUniqueId();
}

#ifdef CEREBROS
void GenRequests(UniqueIdServiceClient** clients,
								 RandomGenerator *randGen) {

  for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new UniqueIdServiceClient(randGen);
  }
}
#else
void GenRequests(MyThriftClient<UniqueIdServiceClient> *clientPtr,
								 RandomGenerator *randGen){

	auto client = clientPtr->GetClient();
	for (int i = 0; i < NUM_MSGS_PER_CLIENT; i++) {
		client->uploadUniqueId_args = new UniqueIdService_UploadUniqueId_args(randGen);
		auto args = client->uploadUniqueId_args;
  	client->send_UploadUniqueId(args->req_id, args->post_type);
	}

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;  
}
#endif

void GenAndProcessReqs(rpcNUMAContext* ctx,
											 int tid,
											 std::shared_ptr<UniqueIdHandler> handler,
											 NebulaClientPool<ComposePostServiceClient> *clientPool) {

  // LOG(warning) << "User TID: " << tid << " TID: " << std::this_thread::get_id();
  RandomGenerator randGen(tid);

// Generate fake requests
#ifdef CEREBROS
	UniqueIdServiceClient *clients[NUM_TEMPLATE_CLIENTS];
  GenRequests(clients, &randGen);

	std::shared_ptr<UniqueIdServiceCerebrosProcessor> proc = 
		std::make_shared<UniqueIdServiceCerebrosProcessor>(handler);

	auto processor = new CerebrosProcessor<UniqueIdServiceCerebrosProcessor, UniqueIdServiceClient>(ctx, tid, proc, clients);

#else
	uint64_t buffer_size = NUM_MSGS_PER_CLIENT * BASE_BUFFER_SIZE;
  
  MyThriftClient<UniqueIdServiceClient>* clients[NUM_TEMPLATE_CLIENTS];

	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<UniqueIdServiceClient>(buffer_size);
		GenRequests(clients[i], &randGen);
	}

	std::shared_ptr<UniqueIdServiceProcessor> proc = 
		std::make_shared<UniqueIdServiceProcessor>(handler);

	auto processor = new NebulaThriftProcessor<UniqueIdServiceProcessor, UniqueIdServiceClient>(ctx, tid, proc, clients);

#endif
  

	auto f2cMap = clientPool->AddToPool(ctx->getQP(tid));
	ComposePostServiceClient::InitializeFuncMapComposePost(f2cMap,
																												 &randGen,
																												 NUM_TEMPLATE_CLIENTS,
																												 NUM_MSGS_PER_CLIENT,
																												 BUFFER_SIZE);

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

	// Wait for all the threads to reach here & then pause.
  pthread_barrier_wait(&barrier);

  if (tid == 0) {
		fprintf(stdout,"Init done! Ready to start execution!\n");
    ctx->readyForTiming();

    start = true;
    // LOG(warning) << "Process Phase Started!!";
  }

  while(!start);

  uint64_t count = 1;

  // Process loop
	while (count <= num_iterations) {
    processor->process(count);

    // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;
    // ClientRecvUniqueId(uniqueIdClient);

    count++;
  }

  #ifdef SW
    processor->printSWResults();
  #endif

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

  // if (tid == max_tid)
  //   LOG(warning) << "Process Phase finished!";
}

int main(int argc, char *argv[]) {
  init_logger();

  uint64_t num_threads, num_nodes;

  if (argc != 4) {
    cout << "Invalid input! Usage: " << argv[0] << " <total # nodes> <num_threads> <iterations> \n" << endl;
    exit(-1);
  } else {
    num_nodes = atoi(argv[1]);
    num_threads = atoi(argv[2]);
    num_iterations = atoi(argv[3]);
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

	NebulaClientPool<ComposePostServiceClient> compose_post_pool("compose-post", BUFFER_SIZE, rpcContext);
  std::shared_ptr<UniqueIdHandler> handler = std::make_shared<UniqueIdHandler>(
                                              &thread_lock, machine_id, &compose_post_pool);
  
  std::thread processThreads[num_threads+1];

  int coreID = 0;
  while (coreID <= num_threads) {

    if (coreID == 1) { //skip this core, this will only run the main thread
      coreID++;
      continue;
    }

    processThreads[coreID] = std::thread(GenAndProcessReqs,
                                          rpcContext,
                                          coreID,
                                          handler,
																					&compose_post_pool);

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
