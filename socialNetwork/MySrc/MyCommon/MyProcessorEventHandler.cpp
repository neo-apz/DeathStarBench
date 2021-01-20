#include "MyProcessorEventHandler.h"

#include <fstream>
#include <iostream>

using namespace std;

namespace apache {
namespace thrift {

void MyProcessorEventHandler::preRead(void* ctx, const char* fn_name) {
    #ifdef __aarch64__
      DISPATCH_END();
      READ_BEGIN();
    #endif

    #ifdef SW
    this->disSW_->stop();
    this->readSW_.start();
    #endif
}

void MyProcessorEventHandler::postRead(void* ctx, const char* fn_name, uint32_t bytes) {

    #ifdef __aarch64__
      READ_END();
      SERVICE_BEGIN();
    #endif
    
    #ifdef SW
    this->readSW_.stop();
    this->servSW_.start();
    #endif
}

void MyProcessorEventHandler::preWrite(void* ctx, const char* fn_name) {

    #ifdef __aarch64__
      SERVICE_END();
      WRITE_BEGIN();
    #endif
    
    #ifdef SW
    this->servSW_.stop();
    this->writeSW_.start();
    #endif
}

void MyProcessorEventHandler::postWrite(void* ctx, const char* fn_name, uint32_t bytes) {
    
    #ifdef __aarch64__
      WRITE_END();
    #endif

    #ifdef SW
    this->writeSW_.stop();
    #endif
}

void MyProcessorEventHandler::printResults() {
    this->readSW_.post_process();
    double meanReadTime = this->readSW_.mean() * 1.0;
    cout << "Deser. Latency (us): " << meanReadTime / 1000 << endl;

    this->writeSW_.post_process();
    double meanWriteTime = this->writeSW_.mean() * 1.0;
    cout << "Ser. Latency (us): " << meanWriteTime / 1000 << endl;

    this->servSW_.post_process();
    double meanServTime = this->servSW_.mean() * 1.0;
    cout << "Serv. Latency (us): " << meanServTime / 1000 << endl;
}

}
}