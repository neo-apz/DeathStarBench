#include "UniqueIdHandler.h"

#define BASE_BUFFER_SIZE         50
#include <utils.h>

#include <NebulaClientPool.h>

std::mutex thread_lock;
std::string machine_id;

// void ClientRecvUniqueId(MyThriftClient<UniqueIdServiceClient> *uniqueIdClient){
  
//   uniqueIdClient->Connect();
//   auto client = uniqueIdClient->GetClient();

//   client->recv_UploadUniqueId();
// }

void initPools(RandomGenerator* randGen, rpcNUMAContext* ctx,
							 std::shared_ptr<UniqueIdHandler> handler, int tid) {

	auto f2cMap = handler->_compose_client_pool->AddToPool(ctx->getQP(tid));
	ComposePostServiceClient::InitializeFuncMapComposePost(f2cMap, randGen,
													NUM_TEMPLATE_CLIENTS, NUM_MSGS_PER_CLIENT, BUFFER_SIZE);	
}

int main(int argc, char *argv[]) {

	rpcNUMAContext* rpcContext = MainInit(argc, argv);

  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

	NebulaClientPool<ComposePostServiceClient> compose_post_pool("compose-post", BUFFER_SIZE, rpcContext);
  std::shared_ptr<UniqueIdHandler> handler = std::make_shared<UniqueIdHandler>(
                                              &thread_lock,
																							machine_id,
																							&compose_post_pool);
  
  
	init_pools_function<UniqueIdHandler> initFunc = initPools;

	#ifdef CEREBROS
	int ret = MainProcess<UniqueIdServiceClient, UniqueIdHandler, UniqueIdServiceCerebrosProcessor>(rpcContext, handler, initFunc);
	#else
	int ret = MainProcess<UniqueIdServiceClient, UniqueIdHandler, UniqueIdServiceProcessor>(rpcContext, handler, initFunc);
	#endif

  return ret;
}
