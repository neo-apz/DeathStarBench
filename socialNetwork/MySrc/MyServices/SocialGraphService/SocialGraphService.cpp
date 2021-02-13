#include "SocialGraphHandler.h"

#define BASE_BUFFER_SIZE  220
#include <utils.h>

#include <NebulaClientPool.h>


void initPools(RandomGenerator* randGen, rpcNUMAContext* ctx,
							 std::shared_ptr<SocialGraphHandler> handler, int tid) {

	auto f2cMapMongo = handler->_mongo_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapRedis = handler->_redis_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapUser = handler->_user_pool->AddToPool(ctx->getQP(tid));

	FakeMongoClient::InitializeFuncMapMongo(f2cMapMongo, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	FakeRedisClient::InitializeFuncMapRedis(f2cMapRedis, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	UserServiceClient::InitializeFuncMapUser(f2cMapUser, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
}

int main(int argc, char *argv[]) {
  
  rpcNUMAContext* rpcContext = MainInit(argc, argv);

	NebulaClientPool<FakeMongoClient> mongo_pool("mongo", BUFFER_SIZE, rpcContext);
	NebulaClientPool<FakeRedisClient> redis_pool("redis", BUFFER_SIZE, rpcContext);
	NebulaClientPool<UserServiceClient> user_pool("user-client", BUFFER_SIZE, rpcContext);


  std::shared_ptr<SocialGraphHandler> handler = std::make_shared<SocialGraphHandler>(
                                              &mongo_pool,
																							&redis_pool,
																							&user_pool);
  
	
	init_pools_function<SocialGraphHandler> initFunc = initPools;

	#ifdef CEREBROS
	int ret = MainProcess<MySocialGraphServiceClient, SocialGraphHandler, MySocialGraphServiceCerebrosProcessor>(rpcContext, handler, initFunc);
	#else
	int ret = MainProcess<MySocialGraphServiceClient, SocialGraphHandler, MySocialGraphServiceProcessor>(rpcContext, handler, initFunc);
	#endif

  return ret;
}

// struct MsgType {
//   enum type {
//     GET_FOLLOWERS = 0,
//     GET_FOLLOWEES = 1,
//     FOLLOW = 2,
//     UNFOLLOW = 3,
//     FOLLOW_UNAME = 4,
//     UNFOLLOW_UNAME = 5,
//     IN_USER = 6,

//     SIZE = 7
//   };
// };

// int chances[] = {0, 0, 0, 0,
//                  1, 1, 1, 1,
//                  2, 2,
//                  3, 3,
//                  4, 4,
//                  5, 5};
//                 //  6};
// int chances_size = 16;

// void ClientSendReq(
//   MyThriftClient<MySocialGraphServiceClient> *clientPtr,
//   int64_t req_id, RandomGenerator* randGen){
  
//   clientPtr->Connect();
//   auto client = clientPtr->GetClient();

//   MsgType::type msg_type = (MsgType::type) chances[randGen->getUInt32(chances_size)];

//   switch (msg_type) {
//     case MsgType::GET_FOLLOWERS:
//       {
//         int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
//         client->send_GetFollowers(req_id, user_id);
//       }
//       break;
//     case MsgType::GET_FOLLOWEES:
//       {
//         int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
//         client->send_GetFollowees(req_id, user_id);
//       }
//       break;
//     case MsgType::FOLLOW:
//       {
//         int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
//         int64_t followee_id = randGen->getInt64(RAND_NUM_LIMIT);
//         client->send_Follow(req_id, user_id, followee_id);
//       }
//       break;
//     case MsgType::UNFOLLOW:
//       {
//         int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
//         int64_t followee_id = randGen->getInt64(RAND_NUM_LIMIT);
//         client->send_Unfollow(req_id, user_id, followee_id);
//       }
//       break;
//     case MsgType::FOLLOW_UNAME:
//       {
//         string username = randGen->getAlphaNumericString(12);
//         string f_username = randGen->getAlphaNumericString(12);
//         client->send_FollowWithUsername(req_id, username, f_username);
//       }
//       break;
//     case MsgType::UNFOLLOW_UNAME:
//       {
//         string username = randGen->getAlphaNumericString(12);
//         string f_username = randGen->getAlphaNumericString(12);
//         client->send_UnfollowWithUsername(req_id, username, f_username);
//       }
//       break;
//     case MsgType::IN_USER :
//       {
//         cout << "This is an error, NOT IMPLEMENTED!" << endl;
//         exit(1);
//         // int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
//         // client->send_InsertUser(req_id, user_id);
//       }
//       break;
//     default:
//       cout << "This is an error, wrong message type!" << endl;
//       exit(1);
//   }     
// }

// void ClientRecvReq(MyThriftClient<MySocialGraphServiceClient> *clientPtr, MsgType::type msg_type){
  
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

// void GenAndProcessReqs(MyThriftClient<MySocialGraphServiceClient> *clientPtr,
//                                   std::shared_ptr<MySocialGraphHandler> handler,
//                                   int64_t req_id,
//                                   int tid, int max_tid) {

//   std::shared_ptr<MySocialGraphServiceProcessor> processor =
//       std::make_shared<MySocialGraphServiceProcessor>(handler);

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
//     cout << "Invalid input! Usage: ./MySocialGraphService <num_threads> <iterations> \n" << endl;
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

//   // MyThriftClient<MyComposePostServiceClient>** clients[num_threads];
//   MyThriftClient<MySocialGraphServiceClient>* clients[num_threads];
  
//   std::shared_ptr<MySocialGraphHandler> handler = std::make_shared<MySocialGraphHandler>();

//   std::thread processThreads[num_threads];

//   int64_t req_id_begin = REQ_ID_BEGIN;

//   cpuSet = (cpu_set_t*) malloc(sizeof(cpu_set_t) * num_threads);

//   // buffer_size = BUFFER_SIZE;
//   for (int i = 0; i < num_threads; i++) {
//     // clients[i] = (MyThriftClient<MyComposePostServiceClient>**) malloc(sizeof(MyThriftClient<MyComposePostServiceClient>**) * num_iterations);
//     clients[i] = new MyThriftClient<MySocialGraphServiceClient>(buffer_size);

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