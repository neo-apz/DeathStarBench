#include <thrift/TProcessor.h>

#include "stopwatch.h"

#ifdef __aarch64__
	#include "MagicBreakPoint.h"
#endif

namespace apache {
namespace thrift {


class MyProcessorEventHandler : public TProcessorEventHandler{
public:
    void preRead(void* ctx, const char* fn_name) override;
    void postRead(void* ctx, const char* fn_name, uint32_t bytes) override;
    void preWrite(void* ctx, const char* fn_name) override;
    void postWrite(void* ctx, const char* fn_name, uint32_t bytes) override;
		void handlerError(void* ctx, const char* fn_name) override;

    MyProcessorEventHandler () {}

		MyProcessorEventHandler (Stopwatch<std::chrono::nanoseconds> *disSW) : disSW_(disSW) {}
		void printResults();

		Stopwatch<std::chrono::nanoseconds> readSW_;
		Stopwatch<std::chrono::nanoseconds> writeSW_;

		Stopwatch<std::chrono::nanoseconds> servSW_;

		Stopwatch<std::chrono::nanoseconds> *disSW_;
};

}
}