#include "core_schedule.h"

int PinToCore(std::thread *thread) {
    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);
    schedulerLock.lock();
    int newCoreId = currentCoreId;

    #ifdef __aarch64__
    currentCoreId += 1;
    #else
    currentCoreId += 2;
    #endif
    
    schedulerLock.unlock();
    CPU_SET(newCoreId, &cpuSet);
    pthread_setaffinity_np(thread->native_handle(), sizeof(cpu_set_t), &cpuSet);

    return newCoreId;
}
