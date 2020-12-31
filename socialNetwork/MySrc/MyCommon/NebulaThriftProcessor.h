#ifndef SOCIAL_NETWORK_MICROSERVICES_NEBULATHRIFTPROCESSOR_H
#define SOCIAL_NETWORK_MICROSERVICES_NEBULATHRIFTPROCESSOR_H

#include <thrift/TDispatchProcessor.h>

#include "son-common/libsonuma/cer_rpc.h"
#include "MyThriftClient.h"

#ifdef SW
  #include "stopwatch.h"
	#include "MyProcessorEventHandler.h"
#endif

#if defined(__aarch64__) || defined(FLEXUS)
    #include "MagicBreakPoint.h"
#endif

using namespace ::apache::thrift;

template<class TThriftProcessor, class TThriftClient>
class NebulaThriftProcessor {
 public:
//   CerThriftClient(uint32_t sz);
//   CerThriftClient(uint8_t* cltIbuf, uint32_t ISz, uint8_t* cltObuf, uint32_t OSz);

	NebulaThriftProcessor(rpcNUMAContext* ctx,
												int id,
												std::shared_ptr<TThriftProcessor> processor);

//   CerThriftClient(const CerThriftClient &) = delete;
//   CerThriftClient &operator=(const CerThriftClient &) = delete;
//   CerThriftClient(CerThriftClient<TThriftClient> &&) = default;
//   CerThriftClient &operator=(CerThriftClient &&) = default;

  ~NebulaThriftProcessor();

	bool process(uint64_t count);

	#ifdef SW
		void printSWResults();
	#endif

 private:
	std::shared_ptr<TThriftProcessor> _processor;
	stdcxx::shared_ptr<protocol::TProtocol> _srvInProt;
	stdcxx::shared_ptr<protocol::TProtocol> _srvOutProt;
	MyThriftClient<TThriftClient>* _clients;

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


#ifdef SW
	Stopwatch<std::chrono::nanoseconds> _headerSW;
	Stopwatch<std::chrono::nanoseconds> _disSW;
	std::shared_ptr<MyProcessorEventHandler> _eventHandler;
#endif

	void _getRequest();
	bool _internalProcess();
	void _respond();	
};

#endif //SOCIAL_NETWORK_MICROSERVICES_NEBULATHRIFTPROCESSOR_H
