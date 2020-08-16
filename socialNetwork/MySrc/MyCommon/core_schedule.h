#ifndef CORE_SCHEDULE_H
#define CORE_SCHEDULE_H

#include <thread>
#include "MyLock.h"

static int currentCoreId = 0;
static int currentCoreIdOtherSocket = 1;
static MyLock schedulerLock;

int PinToCore(std::thread *thread, bool sameSocket=true);

#endif // CORE_SCHEDULE_H
