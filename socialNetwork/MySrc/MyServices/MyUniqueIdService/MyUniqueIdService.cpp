#include <utils.h>
#include <NebulaThriftProcessor.h>
#include <RandomGenerator.h>
#include <NebulaClientPool.h>

#include "MyUniqueIdHandler.h"

using namespace my_social_network;

using namespace std;

uint64_t num_iterations;
std::mutex thread_lock;
// MyLock thread_lock;
std::string machine_id;

cpu_set_t *cpuSet;

volatile bool start = false;
pthread_barrier_t barrier;


#define BASE_BUFFER_SIZE         50
#define NUM_TEMPLATE_CLIENTS     20
#define NUM_MSGS_PER_CLIENT      1

void ClientRecvUniqueId(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  client->recv_UploadUniqueId();
}

void GenRequests(MyThriftClient<MyUniqueIdServiceClient> *clientPtr,
								 RandomGenerator *randGen){

	auto client = clientPtr->GetClient();
	for (int i = 0; i < NUM_MSGS_PER_CLIENT; i++) {
		int64_t req_id = randGen->getInt64(0xFFFFFFFFFFFFFF);
	  PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);

  	client->send_UploadUniqueId(req_id, post_type);
	}

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;  
}

void InitializeFunctionMap(FunctionClientMap<MyComposePostServiceClient> *f2cmap, RandomGenerator *randGen) {
	MyThriftClient<MyComposePostServiceClient>** clients = new MyThriftClient<MyComposePostServiceClient>*[NUM_TEMPLATE_CLIENTS];

	// Fill up the clients
	uint64_t buffer_size = NUM_MSGS_PER_CLIENT * BASE_BUFFER_SIZE;
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);
		clients[i]->GetClient()->FakeUploadUniqueId(randGen);
	}

	f2cmap->RegisterFunction(0, clients);
}

void GenAndProcessReqs(rpcNUMAContext* ctx,
											 int tid,
											 std::shared_ptr<MyUniqueIdHandler> handler,
											 NebulaClientPool<MyComposePostServiceClient> *clientPool) {

  // LOG(warning) << "User TID: " << tid << " TID: " << std::this_thread::get_id();
  RandomGenerator randGen(tid);

	auto f2cMap = clientPool->AddToPool();

	InitializeFunctionMap(f2cMap, &randGen);

  uint64_t buffer_size = NUM_MSGS_PER_CLIENT * BASE_BUFFER_SIZE;
  
  MyThriftClient<MyUniqueIdServiceClient>* clients[NUM_TEMPLATE_CLIENTS];

	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<MyUniqueIdServiceClient>(buffer_size);
		GenRequests(clients[i], &randGen);
	}

  std::shared_ptr<MyUniqueIdServiceProcessor> proc = 
		std::make_shared<MyUniqueIdServiceProcessor>(handler);

	auto processor = new NebulaThriftProcessor<MyUniqueIdServiceProcessor, MyUniqueIdServiceClient>(ctx, tid, proc, clients);

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

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

	NebulaClientPool<MyComposePostServiceClient> compose_post_pool("compose-post", BUFFER_SIZE);
  std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
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
