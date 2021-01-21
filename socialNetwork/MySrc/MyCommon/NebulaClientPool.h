#ifndef NEBULACLIENTPOOL_H
#define NEBULACLIENTPOOL_H

#include <mutex>
#include <map>
#include <string>
#include <utility>  

#include "logger.h"

#include "son-common/libsonuma/cer_rpc.h"
#include "FunctionClientMap.h"

using namespace std;

template<class TThriftClient>
class NebulaClientPool {
 public:
  NebulaClientPool(const string &client_type, size_t buff_size, rpcNUMAContext* ctx);
  ~NebulaClientPool();

  NebulaClientPool(const NebulaClientPool&) = delete;
  NebulaClientPool& operator=(const NebulaClientPool&) = delete;
  NebulaClientPool(NebulaClientPool&&) = default;
  NebulaClientPool& operator=(NebulaClientPool&&) = default;

  FunctionClientMap<TThriftClient>* AddToPool(soNUMAQP_T* qp);
	MyThriftClient<TThriftClient>* Get(int fid);

 private:
  map<thread::id,
			pair<FunctionClientMap<TThriftClient> *, soNUMAQP_T*>> _pool;
  string _client_type;
	size_t _buff_size;
  mutex _mtx;
	rpcNUMAContext* _ctx;
};

template<class TThriftClient>
NebulaClientPool<TThriftClient>::NebulaClientPool(
	const string &client_type, size_t buff_size, rpcNUMAContext* ctx) :
	_client_type(client_type), _buff_size(buff_size), _ctx(ctx) {}

template<class TThriftClient>
NebulaClientPool<TThriftClient>::~NebulaClientPool() {
  // for (auto element : _pool) {
	// 	delete element;
  // }
}

template<class TThriftClient>
FunctionClientMap<TThriftClient>* NebulaClientPool<TThriftClient>::AddToPool(soNUMAQP_T* qp) {
	thread::id tid = this_thread::get_id();

	auto clientMap = new FunctionClientMap<TThriftClient>();

	pair<FunctionClientMap<TThriftClient> *, soNUMAQP_T*> clientPair = {clientMap, qp};

	_mtx.lock();
	_pool[tid] = clientPair;
	_mtx.unlock();

	return _pool.at(tid).first;
}

template<class TThriftClient>
MyThriftClient<TThriftClient>* NebulaClientPool<TThriftClient>::Get(int fid) {
	thread::id tid = this_thread::get_id();

	try{
		pair<FunctionClientMap<TThriftClient> *, soNUMAQP_T*> clientPair = _pool.at(tid);
		int cid = 0;

		auto clientMap = clientPair.first;
		soNUMAQP_T* qp = clientPair.second;

		RPCOutReq req;
		req.ctx_id = _ctx->getCtxId();
		req.from = _ctx->getNodeId();
		req.to = 100; // TODO check this later
		req.param_ptr = (uint64_t) &cid;

		RPCInReq resp;

		// Send out the request (to get a cid from the NI)
		_ctx->cerSendRPC(qp->wq, req, 1);
		
		// Get a response
		_ctx->cerRecvRPC(qp->wq, qp->cq, &resp, false);
		cid = resp.req->param_ptr;
		auto client = clientMap->GetClient(fid, cid);

		// Free up the resources!
		_ctx->cerFreeBuff(qp->wq, (uint8_t*) resp.req, 64);

		return client;
	} catch(const std::exception& e) {
		LOG(error) << "Error accessing ClientPair for " <<  _client_type << " : \n"
							 << e.what() << '\n';
		exit(1);
	}
}

#endif //NEBULACLIENTPOOL_H