#ifndef FUNCTIONCLIENTMAP_H
#define FUNCTIONCLIENTMAP_H

#include <map>
#include <string>

#include "MyThriftClient.h"

using namespace std;
using namespace my_social_network;

template<class TThriftClient>
class FunctionClientMap {
 public:
  FunctionClientMap() {};


  void RegisterFunction(int fid, MyThriftClient<TThriftClient> **clients);
	MyThriftClient<TThriftClient>* GetClient(int fid, int cid);

 private:
  map<int, MyThriftClient<TThriftClient> **> _client_map;
};

// template<class TThriftClient>
// FunctionClientMap<TThriftClient>::FunctionClientMap() {

// }

template<class TThriftClient>
void FunctionClientMap<TThriftClient>::RegisterFunction(int fid, MyThriftClient<TThriftClient> **clients) {
	_client_map[fid] = clients;
}


template<class TThriftClient>
MyThriftClient<TThriftClient>* FunctionClientMap<TThriftClient>::GetClient(int fid, int cid) {
	return _client_map[fid][cid];
}


// Helper functions

// #include "../gen-cpp/FakeRedis.h"
// #include "../gen-cpp/FakeRabbitmq.h"
// #include "../gen-cpp/PostStorageService.h"
// #include "../gen-cpp/UserTimelineService.h"

// #include <RandomGenerator.h>

// void InitializeFuncMapRedis(FunctionClientMap<FakeRedisClient> *f2cmap,
// 																RandomGenerator *randGen,
// 																int num_template_clients,
// 																int num_msg_per_client,
// 																int base_buffer_size);

// void InitializeFuncMapPostStorage(FunctionClientMap<PostStorageServiceClient> *f2cmap,
// 																RandomGenerator *randGen,
// 																int num_template_clients,
// 																int num_msg_per_client,
// 																int base_buffer_size);																

// void InitializeFuncMapUserTimeline(FunctionClientMap<UserTimelineServiceClient> *f2cmap,
// 																RandomGenerator *randGen,
// 																int num_template_clients,
// 																int num_msg_per_client,
// 																int base_buffer_size);																

// void InitializeFuncMapRabbitmq(FunctionClientMap<FakeRabbitmqClient> *f2cmap,
// 																RandomGenerator *randGen,
// 																int num_template_clients,
// 																int num_msg_per_client,
// 																int base_buffer_size);																
#endif //FUNCTIONCLIENTMAP_H