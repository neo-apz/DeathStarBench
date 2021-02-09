#include "MyUserHandler.h"

#define BASE_BUFFER_SIZE  220
#include <utils.h>

#include <NebulaClientPool.h>

void initPools(RandomGenerator* randGen, rpcNUMAContext* ctx,
							 std::shared_ptr<UserHandler> handler, int tid) {

	auto f2cMapMemcached = handler->_memcached_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapMongo = handler->_mongo_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapSocialGraph = handler->_socialgraph_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapComposePost = handler->_compose_pool->AddToPool(ctx->getQP(tid));

	FakeMemcachedClient::InitializeFuncMapRedis(f2cMapMemcached, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	FakeMongoClient::InitializeFuncMapPostStorage(f2cMapMongo, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	MySocialGraphServiceClient::InitializeFuncMapUserTimeline(f2cMapSocialGraph, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	ComposePostServiceClient::InitializeFuncMapRabbitmq(f2cMapComposePost, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
}

int main(int argc, char *argv[]) {
  
  rpcNUMAContext* rpcContext = MainInit(argc, argv);

	NebulaClientPool<FakeMemcachedClient> memcached_pool("memcached", BUFFER_SIZE, rpcContext);
	NebulaClientPool<FakeMongoClient> mongo_pool("mongo", BUFFER_SIZE, rpcContext);
	NebulaClientPool<MySocialGraphServiceClient> socialgraph_pool("social-graph", BUFFER_SIZE, rpcContext);
	NebulaClientPool<ComposePostServiceClient> compose_pool("compose-post", BUFFER_SIZE, rpcContext);

	std::string machine_id;
  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }
	std::mutex thread_lock;
	std::string secret = "secret";


  std::shared_ptr<UserHandler> handler = std::make_shared<UserHandler>(
                                              &thread_lock,
																							machine_id,
																							secret,
																							&memcached_pool,
																							&mongo_pool,
																							&socialgraph_pool,
																							&compose_pool);
  
	
	init_pools_function<UserHandler> initFunc = initPools;

	#ifdef CEREBROS
	int ret = MainProcess<UserServiceClient, UserHandler, UserServiceCerebrosProcessor>(rpcContext, handler, initFunc);
	#else
	int ret = MainProcess<UserServiceClient, UserHandler, UserServiceProcessor>(rpcContext, handler, initFunc);
	#endif

  return ret;
}

// struct MsgType {
//   enum type {
//     REG_USER = 0,
//     REG_USER_ID = 1,
//     UP_CRT_ID = 2,
//     UP_CRT_UNAME = 3,
//     GET_UID = 4,
//     LOGIN = 5,

//     SIZE = 6
//   };
// };

// int chances[] = {0,
//                  1,
//                  2, 2, 2, 2, 2, 2, 2, 2,
//                  3, 3, 3, 3, 3, 3, 3, 3,
//                  4, 4, 4, 4, 4};
//                 //  5, 5};
// int chances_size = 23;

// void ClientSendReq(
//   MyThriftClient<MyUserServiceClient> *clientPtr,
//   int64_t req_id, RandomGenerator* randGen){
  
//   clientPtr->Connect();
//   auto client = clientPtr->GetClient();

//   MsgType::type msg_type = (MsgType::type) chances[randGen->getUInt32(chances_size)];

//   switch (msg_type) {
//     case MsgType::REG_USER:
//       {
//         string first_name = randGen->getAlphaString(8);
//         string last_name = randGen->getAlphaString(12);
//         string username = randGen->getAlphaNumericString(12);
//         string password = randGen->getAlphaNumericString(12);
//         client->send_RegisterUser(req_id, first_name, last_name, username, password);
//       }
//       break;
//     case MsgType::REG_USER_ID:
//       {
//         int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
//         string first_name = randGen->getAlphaString(8);
//         string last_name = randGen->getAlphaString(12);
//         string username = randGen->getAlphaNumericString(12);
//         string password = randGen->getAlphaNumericString(12);
//         client->send_RegisterUserWithId(req_id, first_name, last_name, username, password, user_id);
//       }
//       break;
//     case MsgType::UP_CRT_ID:
//       {
//         int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
//         string username = randGen->getAlphaNumericString(12);
//         client->send_UploadCreatorWithUserId(req_id, user_id, username);
//       }
//       break;
//     case MsgType::UP_CRT_UNAME:
//       {
//         string username = randGen->getAlphaNumericString(12);
//         client->send_UploadCreatorWithUsername(req_id, username);
//       }
//       break;
//     case MsgType::GET_UID:
//       {
//         string username = randGen->getAlphaNumericString(12);
//         client->send_GetUserId(req_id, username);
//       }
//       break;
//     case MsgType::LOGIN:
//       {
//         cout << "This is an error, NOT IMPLEMENTED!" << endl;
//         exit(1);
//         // string username = randGen->getAlphaNumericString(12);
//         // string password = randGen->getAlphaNumericString(12);
//         // client->send_Login(req_id, username, password);
//       }
//       break;
//     default:
//       cout << "This is an error, wrong message type!" << endl;
//       exit(1);
//   }     
// }

// void ClientRecvReq(MyThriftClient<MyUserServiceClient> *clientPtr, MsgType::type msg_type){
  
//   clientPtr->Connect();
//   auto client = clientPtr->GetClient();

//   // switch (msg_type)
//   // {
//   //   case MsgType::GET_FOLLOWERS :
//   //     client->recv_GetFollowees();
//   //     break;
//   //   case MsgType::GET_FOLLOWEES :
//   //     client->recv_UploadMedia();
//   //     break;
//   //   case MsgType::FOLLOW :
//   //     client->recv_UploadUniqueId();
//   //     break;
//   //   case MsgType::UNFOLLOW :
//   //     client->recv_UploadCreator();
//   //     break;
//   //   case MsgType::FOLLOW_UNAME :
//   //     client->recv_UploadUrls();
//   //     break;
//   //   case MsgType::UNFOLLOW_UNAME :
//   //     client->recv_UploadUserMentions();
//   //     break;    
//   //   case MsgType::IN_USER :
//   //     client->recv_UploadUserMentions();
//   //     break;    
//   //   default:
//   //     cout << "This is an error, wrong message type!" << endl;
//   //     exit(1);
//   // } 

// }

// void GenAndProcessReqs(MyThriftClient<MyUserServiceClient> *clientPtr,
//                                   std::shared_ptr<MyUserHandler> handler,
//                                   int64_t req_id,
//                                   int tid, int max_tid) {

//   std::shared_ptr<MyUserServiceProcessor> processor =
//       std::make_shared<MyUserServiceProcessor>(handler);

//   RandomGenerator randGen(tid);

//   uint64_t iter_count = 0;
  
//   // apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> srvIProt, srvOProt;

//   while (iter_count < num_iterations){
//     ClientSendReq(clientPtr, req_id, &randGen);
      
//     iter_count++;
//     req_id++;
//   }

//   // uint8_t* cltIBufPtr, *cltOBufPtr;
//   // uint32_t ISz, OSz;
  
//   // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

//   // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

//   std::shared_ptr<MyProcessorEventHandler> eventHandler;

//   #ifdef SW
//     eventHandler = std::make_shared<MyProcessorEventHandler>(&(processor->disSW));
//   #else
//     eventHandler = std::make_shared<MyProcessorEventHandler>();
//   #endif

//   processor->setEventHandler(eventHandler);

//   auto srvIProt = clientPtr->GetClient()->getOutputProtocol();
//   auto srvOProt = clientPtr->GetClient()->getInputProtocol();

//   pthread_barrier_wait(&barrier);
//   if (tid == max_tid) {
//     #ifdef FLEXUS
//     BREAKPOINT();
//     #endif
//     start = true;
//     // LOG(warning) << "Process Phase Started!!";
//   }

//   while(!start);

//   iter_count = 0;

//   while (iter_count < num_iterations){
//     // srvIProt = clientPtrs[client_count]->GetClient()->getOutputProtocol();
//     // srvOProt = clientPtrs[client_count]->GetClient()->getInputProtocol();

//     // msg_type = (MsgType::type) (count % MsgType::SIZE);

//     // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;
//     // cout << "Process " << count << " finished!" << endl;
    
//     #ifdef __aarch64__
//       PROCESS_BEGIN(iter_count+1);
//     #endif

//     processor->process(srvIProt, srvOProt, nullptr);

//     #ifdef __aarch64__
//       PROCESS_END(iter_count+1);
//     #endif

//     iter_count++;
//   }

//   // if (tid == max_tid)
//   //   LOG(warning) << "Process Phase finished!";

//   // uint8_t* cltIBufPtr, *cltOBufPtr;
//   // uint32_t ISz, OSz;
  
//   // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

//   // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

//   #ifdef SW
//     eventHandler->printResults();
//     processor->headerSW.post_process();
//     processor->disSW.post_process();

//     double headerTime = (processor->headerSW.mean() * 1.0);
//     cout << "AVG HeaderParsing Latency (us): " << headerTime / 1000 << endl;

//     double disTime = (processor->disSW.mean() * 1.0);
//     cout << "AVG Dispatch Latency (us): " << disTime / 1000 << endl;
//   #endif
// }

// int main(int argc, char *argv[]) {
//   init_logger();

//   uint64_t num_threads;

//   if (argc != 3) {
//     cout << "Invalid input! Usage: ./MyUserService <num_threads> <iterations> \n" << endl;
//     exit(-1);
//   } else {
//     num_threads = atoi(argv[1]);
//     num_iterations = atoi(argv[2]);
//   }

//   cpu_set_t  mask;
//   CPU_ZERO(&mask);
//   CPU_SET(0, &mask);
//   sched_setaffinity(0, sizeof(mask), &mask);

//   uint64_t buffer_size = num_iterations * BUFFER_SIZE;

//   pthread_barrier_init(&barrier, NULL, num_threads);

//   std::string machine_id;
//   if (GetMachineId(&machine_id) != 0) {
//     exit(EXIT_FAILURE);
//   }

//   // MyThriftClient<MyComposePostServiceClient>** clients[num_threads];
//   MyThriftClient<MyUserServiceClient>* clients[num_threads];
  
//   std::shared_ptr<MyUserHandler> handler = std::make_shared<MyUserHandler>(
//                                             &thread_lock,
//                                             machine_id,
//                                             secret);

//   std::thread processThreads[num_threads];

//   int64_t req_id_begin = REQ_ID_BEGIN;

//   cpuSet = (cpu_set_t*) malloc(sizeof(cpu_set_t) * num_threads);

//   // buffer_size = BUFFER_SIZE;
//   for (int i = 0; i < num_threads; i++) {
//     // clients[i] = (MyThriftClient<MyComposePostServiceClient>**) malloc(sizeof(MyThriftClient<MyComposePostServiceClient>**) * num_iterations);
//     clients[i] = new MyThriftClient<MyUserServiceClient>(buffer_size);

//     // for (int c = 0; c < num_iterations; c++) {
//     //   clients[i][c] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);
//     // }

//     processThreads[i] = std::thread(GenAndProcessReqs,
//                                       clients[i],
//                                       handler,
//                                       req_id_begin,
//                                       i, num_threads - 1);

//     req_id_begin += num_iterations + 2;

//     CPU_ZERO(&cpuSet[i]);
//     CPU_SET(i+1, &cpuSet[i]);
//     pthread_setaffinity_np(processThreads[i].native_handle(), sizeof(cpu_set_t), &cpuSet[i]);
//   }

//   for (int i = 0; i < num_threads; i++) {
//     processThreads[i].join();
//   }

//   for (int i = 0; i < num_threads; i++) {
//     delete clients[i];
//     // for (int c = 0; c < num_iterations; c++) {
//     //   delete clients[i][c];
//     // }
//   }

//   // std::cout << "Percentage: " << (handler->counter * 100.0) / (num_iterations*num_threads) << std::endl;

//   return 0;
// }