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

#endif //FUNCTIONCLIENTMAP_H