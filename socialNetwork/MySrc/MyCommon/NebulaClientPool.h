#ifndef NEBULACLIENTPOOL_H
#define NEBULACLIENTPOOL_H

#include <mutex>
#include <map>
#include <string>

#include "logger.h"

#include "FunctionClientMap.h"

using namespace std;

template<class TThriftClient>
class NebulaClientPool {
 public:
  NebulaClientPool(const string &client_type, size_t buff_size);
  ~NebulaClientPool();

  NebulaClientPool(const NebulaClientPool&) = delete;
  NebulaClientPool& operator=(const NebulaClientPool&) = delete;
  NebulaClientPool(NebulaClientPool&&) = default;
  NebulaClientPool& operator=(NebulaClientPool&&) = default;

  FunctionClientMap<TThriftClient>* AddToPool();
	MyThriftClient<TThriftClient>* Get(int fid);

 private:
  map<thread::id, FunctionClientMap<TThriftClient> *> _pool;
  string _client_type;
	size_t _buff_size;
  mutex _mtx;
};

template<class TThriftClient>
NebulaClientPool<TThriftClient>::NebulaClientPool(
	const string &client_type, size_t buff_size) :
	_client_type(client_type), _buff_size(buff_size) {}

template<class TThriftClient>
NebulaClientPool<TThriftClient>::~NebulaClientPool() {
  // for (auto element : _pool) {
	// 	delete element;
  // }
}

template<class TThriftClient>
FunctionClientMap<TThriftClient>* NebulaClientPool<TThriftClient>::AddToPool() {
	thread::id tid = this_thread::get_id();

	auto clientMap = new FunctionClientMap<TThriftClient>();

	_mtx.lock();
	_pool[tid] = clientMap;
	_mtx.unlock();

	return _pool[tid];
}

template<class TThriftClient>
MyThriftClient<TThriftClient>* NebulaClientPool<TThriftClient>::Get(int fid) {

	thread::id tid = this_thread::get_id();
  auto clientMap = _pool[tid];

  if (!clientMap){
    LOG(error) << "ClientMap for " +  _client_type + " is NULL!";
		exit(1);
	}

	return clientMap->GetClient(fid);
}

#endif //NEBULACLIENTPOOL_H