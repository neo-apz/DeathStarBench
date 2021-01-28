#ifndef SOCIAL_NETWORK_MICROSERVICES_CEREBROSPROCESSOR_H
#define SOCIAL_NETWORK_MICROSERVICES_CEREBROSPROCESSOR_H

#include <iostream>

#include <thrift/stdcxx.h>

#include "son-common/libsonuma/cer_rpc.h"

#ifdef __aarch64__
	#include "MagicBreakPoint.h"
#endif

template<class TProcessor, class TClient>
class CerebrosProcessor {
 public:

	CerebrosProcessor(rpcNUMAContext* ctx,
										int id,
										std::shared_ptr<TProcessor> processor,
										TClient** clients);

  ~CerebrosProcessor();

	void process(uint64_t count);

 private:
	std::shared_ptr<TProcessor> _processor;
	TClient** _clients;

	int _last_clientId = -1;
	int _last_func_code = -1;
	TClient* _lastClientPtr = nullptr;

	// Nebula-related parameters
	rpcNUMAContext* _ctx;
	uint8_t* _local_buf = nullptr;
	soNUMAQP_T* _my_qp = nullptr;
	RPCInReq _rpc_req;
	RPCOutReq _resp;
	
	size_t freeSizeBytes = 64;
	uint8_t* rbufSlotPtr = nullptr;

	void _getRequest();
	void _internalProcess();
	void _respond();	
};

template<class TProcessor, class TClient>
CerebrosProcessor<TProcessor, TClient>::CerebrosProcessor(
										rpcNUMAContext* ctx,
										int id,
										std::shared_ptr<TProcessor> processor,
										TClient** clients)
										: _ctx(ctx), _processor(processor), _clients(clients) {
  
	// Allocate and register buffer
	_ctx->registerNewLocalBuffer(&_local_buf, id);
  _ctx->registerNewSONUMAQP(id);

	// Allocate and register the QP
  _my_qp = _ctx->getQP(id);

  _resp.ctx_id = _ctx->getCtxId();
  _resp.from = _ctx->getNodeId();
}


template<class TProcessor, class TClient>
CerebrosProcessor<TProcessor, TClient>::~CerebrosProcessor() {
  // TODO: free allocated space
}

template<class TProcessor, class TClient>
void CerebrosProcessor<TProcessor, TClient>::process(uint64_t count){

	#ifdef __aarch64__
		PASS2FLEXUS_MEASURE(0, NETPIPE_END, count);
	#endif
	_getRequest();

	#ifdef __aarch64__
		PROCESS_BEGIN(count);
		PASS2FLEXUS_MEASURE(0, MEASUREMENT, 101); // sets "notification time" in msg_lat.csv
    PASS2FLEXUS_MEASURE(0, NETPIPE_START,count+1);
	#endif
	
	_internalProcess();

	#ifdef __aarch64__
		PASS2FLEXUS_MEASURE(0, MEASUREMENT, 102); // sets "exec_time_synth" in msg_lat.csv
		PROCESS_END(count);
	#endif

	_respond();
}

template<class TProcessor, class TClient>
void CerebrosProcessor<TProcessor, TClient>::_getRequest(){
	
	// Get a new request!
	_ctx->cerRecvRPC(_my_qp->qp_id, _my_qp->wq, _my_qp->cq, &_rpc_req, true);
	_resp.to = _rpc_req.from;
	
	_last_clientId = _rpc_req.req->param_ptr;
	_lastClientPtr = _clients[_last_clientId];
	_last_func_code = _rpc_req.req->func_code;
}

template<class TProcessor, class TClient>
void CerebrosProcessor<TProcessor, TClient>::_internalProcess(){
	_resp.param_ptr = _processor->dispatchCall(_last_func_code, _lastClientPtr);
}

template<class TProcessor, class TClient>
void CerebrosProcessor<TProcessor, TClient>::_respond(){
	// Send out the response
	_ctx->cerSendRPC(_my_qp->wq, _resp, 0);

	// Free up the resources!
	_ctx->cerFreeBuff(_my_qp->wq, (uint8_t*) _rpc_req.req, freeSizeBytes);
}

#endif //SOCIAL_NETWORK_MICROSERVICES_CEREBROSPROCESSOR_H
