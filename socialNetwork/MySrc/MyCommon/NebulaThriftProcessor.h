#ifndef SOCIAL_NETWORK_MICROSERVICES_NEBULATHRIFTPROCESSOR_H
#define SOCIAL_NETWORK_MICROSERVICES_NEBULATHRIFTPROCESSOR_H

#include <iostream>

#include <thrift/TDispatchProcessor.h>

#include "son-common/libsonuma/cer_rpc.h"
#include "MyThriftClient.h"

#include "stopwatch.h"
#include "MyProcessorEventHandler.h"

#ifdef __aarch64__
	#include "MagicBreakPoint.h"
#endif

using namespace ::apache::thrift;
using namespace my_social_network;

template<class TThriftProcessor, class TThriftClient>
class NebulaThriftProcessor {
 public:
//   CerThriftClient(uint32_t sz);
//   CerThriftClient(uint8_t* cltIbuf, uint32_t ISz, uint8_t* cltObuf, uint32_t OSz);

	NebulaThriftProcessor(rpcNUMAContext* ctx,
												int id,
												std::shared_ptr<TThriftProcessor> processor,
												MyThriftClient<TThriftClient>** clients);

//   CerThriftClient(const CerThriftClient &) = delete;
//   CerThriftClient &operator=(const CerThriftClient &) = delete;
//   CerThriftClient(CerThriftClient<TThriftClient> &&) = default;
//   CerThriftClient &operator=(CerThriftClient &&) = default;

  ~NebulaThriftProcessor();

	bool process(uint64_t count);

	void printSWResults();

 private:
	std::shared_ptr<TThriftProcessor> _processor;
	stdcxx::shared_ptr<protocol::TProtocol> _srvInProt;
	stdcxx::shared_ptr<protocol::TProtocol> _srvOutProt;
	MyThriftClient<TThriftClient>** _clients;

	int _last_clientId = -1;
	MyThriftClient<TThriftClient>* _lastClientPtr = nullptr;

	// Nebula-related parameters
	rpcNUMAContext* _ctx;
	uint8_t* _local_buf = nullptr;
	soNUMAQP_T* _my_qp = nullptr;
	RPCInReq _rpc_req;
	RPCOutReq _resp;
	// TODO: figure out how to set these two!
	size_t freeSizeBytes = 64;
	uint8_t* rbufSlotPtr = nullptr;


	Stopwatch<std::chrono::nanoseconds> _headerSW;
	Stopwatch<std::chrono::nanoseconds> _disSW;
	std::shared_ptr<MyProcessorEventHandler> _eventHandler;

	void _getRequest();
	bool _internalProcess();
	void _respond();	
};


template<class TThriftProcessor, class TThriftClient>
NebulaThriftProcessor<TThriftProcessor, TThriftClient>::NebulaThriftProcessor(
											rpcNUMAContext* ctx,
											int id,
											std::shared_ptr<TThriftProcessor> processor,
											MyThriftClient<TThriftClient>** clients)
												: _ctx(ctx), _processor(processor), _clients(clients) {
  
	_ctx->registerNewLocalBuffer(&_local_buf, id);
  _ctx->registerNewSONUMAQP(id);

  _my_qp = _ctx->getQP(id);

  _resp.ctx_id = _ctx->getCtxId();
  _resp.from = _ctx->getNodeId();

	_eventHandler = std::make_shared<MyProcessorEventHandler>(&_disSW);
	_processor->setEventHandler(_eventHandler);
	
}


template<class TThriftProcessor, class TThriftClient>
NebulaThriftProcessor<TThriftProcessor, TThriftClient>::~NebulaThriftProcessor() {
  // TODO: free allocated space
}

template<class TThriftProcessor, class TThriftClient>
bool NebulaThriftProcessor<TThriftProcessor, TThriftClient>::process(uint64_t count){

	_getRequest();

	#ifdef __aarch64__
		PROCESS_BEGIN(count);
	#endif
	
	bool retVal = _internalProcess();

	#ifdef __aarch64__
		PROCESS_END(count);
	#endif

	_respond();

	return retVal;
}

template<class TThriftProcessor, class TThriftClient>
void NebulaThriftProcessor<TThriftProcessor, TThriftClient>:: printSWResults(){
	_eventHandler->printResults();
	_headerSW.post_process();
	_disSW.post_process();

	double headerTime = (_headerSW.mean() * 1.0);
	std::cout << "AVG HeaderParsing Latency (us): " << headerTime / 1000 << std::endl;

	double disTime = (_disSW.mean() * 1.0);
	std::cout << "AVG Dispatch Latency (us): " << disTime / 1000 << std::endl;
}

template<class TThriftProcessor, class TThriftClient>
void NebulaThriftProcessor<TThriftProcessor, TThriftClient>::_getRequest(){
	
	// Get a new request!
	_ctx->cerRecvRPC(_my_qp->qp_id, _my_qp->wq, _my_qp->cq, &_rpc_req, true);
	_resp.to = _rpc_req.from;
	
	_last_clientId = _rpc_req.req->param_ptr;
	_lastClientPtr = _clients[_last_clientId];
	TThriftClient* client = _lastClientPtr->GetClient();

	_srvInProt = client->getOutputProtocol();
	_srvOutProt = client->getInputProtocol();
}

template<class TThriftProcessor, class TThriftClient>
bool NebulaThriftProcessor<TThriftProcessor, TThriftClient>::_internalProcess(){
	#ifdef __aarch64__
		HEADER_BEGIN();
	#endif

	#ifdef SW
		_headerSW.start();
	#endif


	std::string fname;
	protocol::TMessageType mtype;
	int32_t seqid;
	_srvInProt->readMessageBegin(fname, mtype, seqid);
	
	#ifdef __aarch64__
		HEADER_END();
		DISPATCH_BEGIN();
	#endif

	#ifdef SW
		_headerSW.stop();
		_disSW.start();
	#endif

	if (mtype != protocol::T_CALL && mtype != protocol::T_ONEWAY) {
		GlobalOutput.printf("received invalid message type %d from client", mtype);
		return false;
	}

	return _processor->dispatchCall(_srvInProt.get(), _srvOutProt.get(), fname, seqid, nullptr);
}

template<class TThriftProcessor, class TThriftClient>
void NebulaThriftProcessor<TThriftProcessor, TThriftClient>::_respond(){
	
	// This should actually refer to the buffer pointer!
	_resp.param_ptr = (uint64_t) _lastClientPtr->GetClient()->getInputProtocol().get();
	
	// Send out the response
	_ctx->cerSendRPC(_my_qp->wq, _resp, 0);

	// Free up the resources!
	_ctx->cerFreeBuff(_my_qp->wq, (uint8_t*) _rpc_req.req, freeSizeBytes);

	// Reset the client msg buffer
	_lastClientPtr->ResetBuffers(false, true);
}

#endif //SOCIAL_NETWORK_MICROSERVICES_NEBULATHRIFTPROCESSOR_H
