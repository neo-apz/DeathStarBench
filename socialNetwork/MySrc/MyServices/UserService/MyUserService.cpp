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

	FakeMemcachedClient::InitializeFuncMapMemcached(f2cMapMemcached, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	FakeMongoClient::InitializeFuncMapMongo(f2cMapMongo, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	MySocialGraphServiceClient::InitializeFuncMapSocialGraph(f2cMapSocialGraph, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	ComposePostServiceClient::InitializeFuncMapComposePost(f2cMapComposePost, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
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
