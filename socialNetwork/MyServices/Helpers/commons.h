//
// Created by Arash on 2019-07-17.
//

#ifndef SOCCIALNET_MYSERVICES_COMMONS_H
#define SOCCIALNET_MYSERVICES_COMMONS_H


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/stdcxx.h>

#if defined(FLEXUS)
#include "../flexus/flexus.h"
#endif


// #include "../utils.h"
#include <string>
#include <fstream>
#include <iostream>

// This is not thread safe!!
static struct timeval tv;

typedef struct stopwatch {
  uint64_t start = 0;
  uint64_t stop = 0;
  uint64_t total = 0;
  uint64_t count = 0;
} stopwatch;


uint64_t now();
void sw_start(stopwatch* sw);
void sw_stop(stopwatch* sw, bool clear=false);
// double sw_getAVG(stopwatch sw);

#endif //SOCCIALNET_MYSERVICES_COMMONS_H


