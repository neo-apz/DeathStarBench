#ifndef FUNCTIONCLIENTMAP_H
#define FUNCTIONCLIENTMAP_H

#include <map>
#include <string>

#ifdef CEREBROS

#else
#include "MyThriftClient.h"
#endif

using namespace std;
using namespace my_social_network;

template<typename TThriftClient>
using fake_resp_gen_func = void (*)(TThriftClient*, uint64_t); // type for conciseness

template<class TThriftClient>
class FunctionClientMap {
 public:
  FunctionClientMap() {};

	#ifdef CEREBROS
	void RegisterFunction(int fid, TThriftClient **clients);
	TThriftClient* GetClient(int fid, int cid);
	#else
	void RegisterFunction(int fid, MyThriftClient<TThriftClient> **clients);
	MyThriftClient<TThriftClient>* GetClient(int fid, int cid);
	#endif

	void InitMap(
				fake_resp_gen_func<TThriftClient> f,
				uint64_t fid,
				RandomGenerator *randGen,
				uint64_t num_template_clients,
				uint64_t num_msg_per_client,
				uint64_t base_buffer_size);

 private:
 	#ifdef CEREBROS
	map<int, TThriftClient**> _client_map;
 	#else
  map<int, MyThriftClient<TThriftClient> **> _client_map;
	#endif
};

// template<class TThriftClient>
// FunctionClientMap<TThriftClient>::FunctionClientMap() {

// }

template<class TThriftClient>
#ifdef CEREBROS
void FunctionClientMap<TThriftClient>::RegisterFunction(int fid, TThriftClient **clients) {
#else
void FunctionClientMap<TThriftClient>::RegisterFunction(int fid, MyThriftClient<TThriftClient> **clients) {
#endif
	_client_map[fid] = clients;
}


template<class TThriftClient>
#ifdef CEREBROS
TThriftClient* FunctionClientMap<TThriftClient>::GetClient(int fid, int cid) {
#else
MyThriftClient<TThriftClient>* FunctionClientMap<TThriftClient>::GetClient(int fid, int cid) {
#endif
	return _client_map[fid][cid];
}

template<class TThriftClient>
void FunctionClientMap<TThriftClient>::InitMap(
				fake_resp_gen_func<TThriftClient> resp_gen_func,
				uint64_t fid,
				RandomGenerator *randGen,
				uint64_t num_template_clients,
				uint64_t num_msg_per_client,
				uint64_t base_buffer_size
				) {

#ifdef CEREBROS
	auto clients = new TThriftClient*[num_template_clients];
#else
	auto clients = new MyThriftClient<TThriftClient>*[num_template_clients];
#endif

	uint64_t buffer_size = num_msg_per_client * base_buffer_size;

	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		
		#ifdef CEREBROS
		clients[i] = new TThriftClient();
		clients[i]->initResults(randGen);
		
		#else
		clients[i] = new MyThriftClient<TThriftClient>(buffer_size);
		clients[i]->GetClient()->initResults(randGen);
		resp_gen_func(clients[i]->GetClient(), fid);
		#endif
	}

	this->RegisterFunction(fid, clients);	
}

#endif //FUNCTIONCLIENTMAP_H