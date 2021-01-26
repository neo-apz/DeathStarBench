#include <utils.h>
#include <NebulaThriftProcessor.h>
#include <RandomGenerator.h>
#include <NebulaClientPool.h>

#include "MyComposePostHandler.h"

using namespace my_social_network;
using namespace std;

uint64_t num_iterations;
cpu_set_t *cpuSet;

volatile bool start = false;
pthread_barrier_t barrier;

#define BASE_BUFFER_SIZE  220
#define NUM_TEMPLATE_CLIENTS     20
#define NUM_MSGS_PER_CLIENT      1

#define REQ_ID_BEGIN 0xFFFFFFFFFFFF

struct MsgType {
  enum type {
    USER_MENTIONS = 0,
    TEXT = 1,
    MEDIA = 2,
    UNIQUE_ID = 3,
    CREATOR = 4,
    URLS = 5,
    
    SIZE = 6
  };
};

void ClientSendComposePost(
  MyThriftClient<ComposePostServiceClient> *clientPtr,
  int64_t req_id, MsgType::type msg_type, RandomGenerator* randGen){
  
  clientPtr->Connect();
  auto client = clientPtr->GetClient();

  switch (msg_type) {
    case MsgType::TEXT:
      {
        string str = randGen->getRandText();
        client->send_UploadText(req_id, str);
      }
      break;
    case MsgType::MEDIA:
      {
        std::vector<Media> media_vector;
        uint32_t iters = randGen->getUInt32(1, 2);
        for(int i=0; i < iters; i++){
          Media media(randGen);
          media_vector.emplace_back(media);
        }
        client->send_UploadMedia(req_id, media_vector);
      }
      break;
    case MsgType::UNIQUE_ID:
      {
        int64_t id = randGen->getInt64(RAND_NUM_LIMIT);
        PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);
        client->send_UploadUniqueId(req_id, id, post_type);
      }
      break;
    case MsgType::CREATOR :
      {
        Creator creator(randGen);
        client->send_UploadCreator(req_id, creator);
      }
      break;
    case MsgType::URLS :
      {
        std::vector<Url> urls;
        uint32_t iters = randGen->getUInt32(1, 2);
        for (int i = 0; i < iters; i++){
          Url url(randGen);
          urls.emplace_back(url);
        }
        client->send_UploadUrls(req_id, urls);
      }
      break;
    case MsgType::USER_MENTIONS :
      {
        std::vector<UserMention> user_mentions;
        uint32_t iters = randGen->getUInt32(1, 2);
        for (int i = 0; i < iters; i++){
          UserMention user_mention(randGen);
          user_mentions.emplace_back(user_mention);
        }
        client->send_UploadUserMentions(req_id, user_mentions);
      }
      break;    
    default:
      cout << "This is an error, wrong message type!" << endl;
      exit(1);
  }     
}


void GenRequests(MyThriftClient<ComposePostServiceClient> *clientPtr,
								 RandomGenerator *randGen){

	for (int i = 0; i < NUM_MSGS_PER_CLIENT; i++) {
		int64_t req_id = randGen->getInt64(0xFFFFFFFFFFFFFF);
	  MsgType::type msg_type = (MsgType::type) randGen->getInt64(0, MsgType::SIZE-1);

		ClientSendComposePost(clientPtr, req_id, msg_type, randGen);
	}

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;  
}

typedef void (*fakeFunc)(void); // type for conciseness


// void InitializeFunctionMap(FunctionClientMap<FakeRedisClient> *f2cmap,
// 													 RandomGenerator *randGen,
// 													 void (*fakeFunc)(RandomGenerator*),
// 													 FakeRedisClient::FuncType::type funcType) {

// 	uint64_t buffer_size = NUM_MSGS_PER_CLIENT * BASE_BUFFER_SIZE;

// 	MyThriftClient<FakeRedisClient>** clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
// 	// Fill up the clients
// 	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
// 		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
// 		clients[i]->GetClient()->(*fakeFunc)(randGen);
// 	}
// 	f2cmap->RegisterFunction(funcType, clients);
// }

void InitializeFunctionMapRedis(FunctionClientMap<FakeRedisClient> *f2cmap, RandomGenerator *randGen) {

	// InitializeFunctionMap(f2cmap, randGen, FakeHSetCreator, FakeRedisClient::FuncType::HS_CREATOR);

	uint64_t buffer_size = NUM_MSGS_PER_CLIENT * BASE_BUFFER_SIZE;

	MyThriftClient<FakeRedisClient>** clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetCreator(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_CREATOR, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetText(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_TEXT, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetMedia(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_MEDIA, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetPostId(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_POST_ID, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetPostType(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_POST_TYPE, clients);
	
	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetUrls(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_URLS, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetUserMentions(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_USER_MENTIONS, clients);


	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetCreator(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_CREATOR, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetText(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_TEXT, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetMedia(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_MEDIA, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetPostId(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_POST_ID, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetPostType(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_POST_TYPE, clients);
	
	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetUrls(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_URLS, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetUserMentions(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_USER_MENTIONS, clients);

	clients = new MyThriftClient<FakeRedisClient>*[NUM_TEMPLATE_CLIENTS];
	// Fill up the clients
	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHHIncrBy(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::H_INC, clients);
}

void GenAndProcessReqs(rpcNUMAContext* ctx,
											 int tid,
											 std::shared_ptr<ComposePostHandler> handler) {

  // LOG(warning) << "User TID: " << tid << " TID: " << std::this_thread::get_id();
  RandomGenerator randGen(tid);

  uint64_t buffer_size = NUM_MSGS_PER_CLIENT * BASE_BUFFER_SIZE;
  
  MyThriftClient<ComposePostServiceClient>* clients[NUM_TEMPLATE_CLIENTS];

	for (int i = 0; i < NUM_TEMPLATE_CLIENTS; i++) {
		clients[i] = new MyThriftClient<ComposePostServiceClient>(buffer_size);
		GenRequests(clients[i], &randGen);
	}

  std::shared_ptr<ComposePostServiceProcessor> proc = 
		std::make_shared<ComposePostServiceProcessor>(handler);

	auto processor = new NebulaThriftProcessor<ComposePostServiceProcessor, ComposePostServiceClient>(ctx, tid, proc, clients);

	auto f2cMapRedis = handler->_redis_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapPostStorage = handler->_post_storage_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapUserTimeline = handler->_user_timeline_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapRabbitmq = handler->_rabbitmq_pool->AddToPool(ctx->getQP(tid));


	InitializeFunctionMapRedis(f2cMapRedis, &randGen);

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

	NebulaClientPool<FakeRedisClient> redis_pool("redis", BUFFER_SIZE, rpcContext);
	NebulaClientPool<PostStorageServiceClient> post_storage_pool("post-storage-client", BUFFER_SIZE, rpcContext);
	NebulaClientPool<UserTimelineServiceClient> user_timeline_pool("user-timeline-client", BUFFER_SIZE, rpcContext);
	NebulaClientPool<FakeRabbitmqClient> rabbitmq_pool("rabbitmq", BUFFER_SIZE, rpcContext);


  std::shared_ptr<ComposePostHandler> handler = std::make_shared<ComposePostHandler>(
                                              &redis_pool,
																							&post_storage_pool,
																							&user_timeline_pool,
																							&rabbitmq_pool);
  
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
                                          handler);

    CPU_ZERO(&mask);
    CPU_SET(coreID, &mask);
    int error = pthread_setaffinity_np(processThreads[coreID].native_handle(), sizeof(cpu_set_t), &mask);
    if (error) {
      printf("Could not bind thread %d to core %d! (error %d)\n", coreID, coreID, error);
    }
    coreID++;
  }


  int64_t req_id_begin = REQ_ID_BEGIN;
	    req_id_begin += num_iterations + 2;


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