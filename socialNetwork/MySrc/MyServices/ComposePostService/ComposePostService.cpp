#include "ComposePostHandler.h"

#define BASE_BUFFER_SIZE  220
#include <utils.h>

#include <NebulaClientPool.h>

void initPools(RandomGenerator* randGen, rpcNUMAContext* ctx,
							 std::shared_ptr<ComposePostHandler> handler, int tid) {

	auto f2cMapRedis = handler->_redis_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapPostStorage = handler->_post_storage_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapUserTimeline = handler->_user_timeline_pool->AddToPool(ctx->getQP(tid));
	auto f2cMapRabbitmq = handler->_rabbitmq_pool->AddToPool(ctx->getQP(tid));

	FakeRedisClient::InitializeFuncMapRedis(f2cMapRedis, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	PostStorageServiceClient::InitializeFuncMapPostStorage(f2cMapPostStorage, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	UserTimelineServiceClient::InitializeFuncMapUserTimeline(f2cMapUserTimeline, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
	FakeRabbitmqClient::InitializeFuncMapRabbitmq(f2cMapRabbitmq, randGen, NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);
}

int main(int argc, char *argv[]) {
  
  rpcNUMAContext* rpcContext = MainInit(argc, argv);

	NebulaClientPool<FakeRedisClient> redis_pool("redis", BUFFER_SIZE, rpcContext);
	NebulaClientPool<PostStorageServiceClient> post_storage_pool("post-storage-client", BUFFER_SIZE, rpcContext);
	NebulaClientPool<UserTimelineServiceClient> user_timeline_pool("user-timeline-client", BUFFER_SIZE, rpcContext);
	NebulaClientPool<FakeRabbitmqClient> rabbitmq_pool("rabbitmq", BUFFER_SIZE, rpcContext);


  std::shared_ptr<ComposePostHandler> handler = std::make_shared<ComposePostHandler>(
                                              &redis_pool,
																							&post_storage_pool,
																							&user_timeline_pool,
																							&rabbitmq_pool);
  
	
	init_pools_function<ComposePostHandler> initFunc = initPools;

	#ifdef CEREBROS
	int ret = MainProcess<ComposePostServiceClient, ComposePostHandler, ComposePostServiceCerebrosProcessor>(rpcContext, handler, initFunc);
	#else
	int ret = MainProcess<ComposePostServiceClient, ComposePostHandler, ComposePostServiceProcessor>(rpcContext, handler, initFunc);
	#endif

  return ret;
}