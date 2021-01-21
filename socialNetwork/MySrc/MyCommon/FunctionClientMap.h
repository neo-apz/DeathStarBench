#ifndef FUNCTIONCLIENTMAP_H
#define FUNCTIONCLIENTMAP_H

#include <map>
#include <string>

#include "logger.h"

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
	size_t _buff_size;
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

#endif //FUNCTIONCLIENTMAP_H