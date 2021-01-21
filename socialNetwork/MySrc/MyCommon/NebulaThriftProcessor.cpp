#include <iostream>

#include "NebulaThriftProcessor.h"

template<class TThriftProcessor, class TThriftClient>
NebulaThriftProcessor<TThriftProcessor, TThriftClient>::NebulaThriftProcessor(
	rpcNUMAContext* ctx, int id, std::shared_ptr<TThriftProcessor> processor) : _ctx(ctx), _processor(processor) {
  
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
	_ctx->cerRecvRPC(_my_qp->qp_id, _my_qp->wq, _my_qp->cq, &_rpc_req);
	_resp.to = _rpc_req.from;
	
	_last_clientId = _rpc_req.req->param_ptr;
	_lastClientPtr = _clients[_last_clientId];
	TThriftClient* client = _lastClientPtr->GetClient();

	_srvInProt = client->getOutputProtocol();
	_srvOutProt = client->getInputProtocol();
}

template<class TThriftProcessor, class TThriftClient>
bool NebulaThriftProcessor<TThriftProcessor, TThriftClient>::_internalProcess(){
	HEADER_BEGIN();

	#ifdef SW
		_headerSW.start();
	#endif


	std::string fname;
	protocol::TMessageType mtype;
	int32_t seqid;
	_srvInProt->readMessageBegin(fname, mtype, seqid);
	
	HEADER_END();
	DISPATCH_BEGIN();

	#ifdef SW
		_headerSW.stop();
		_disSW.start();
	#endif

	if (mtype != protocol::T_CALL && mtype != protocol::T_ONEWAY) {
		GlobalOutput.printf("received invalid message type %d from client", mtype);
		return false;
	}

	return TThriftProcessor::dispatchCall(_srvInProt.get(), _srvOutProt.get(), fname, seqid, nullptr);
}

template<class TThriftProcessor, class TThriftClient>
void NebulaThriftProcessor<TThriftProcessor, TThriftClient>::_respond(){
	
	_resp.param_ptr = _lastClientPtr->GetClient()->getInputProtocol();
	
	// Send out the response
	_ctx->cerSendRPC(_my_qp->wq, _resp, 0);

	// Free up the resources!
	_ctx->cerFreeBuff(_my_qp->wq, (uint8_t*) _rpc_req.req, freeSizeBytes);

	// Reset the client msg buffer
	_lastClientPtr->ResetBuffers(false, true);
}
