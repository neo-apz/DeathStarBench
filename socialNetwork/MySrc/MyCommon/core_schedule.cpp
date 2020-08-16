#include "core_schedule.h"

int PinToCore(std::thread *thread, bool sameSocket) {
    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);
    schedulerLock.lock();
    int newCoreId = (sameSocket) ? currentCoreId++ : currentCoreIdOtherSocket++;

    #ifdef __aarch64__
    #else
    (sameSocket) ? currentCoreId++ : currentCoreIdOtherSocket++;
    #endif
    
    schedulerLock.unlock();
    CPU_SET(newCoreId, &cpuSet);
    if (thread == 0){
        sched_setaffinity(0, sizeof(cpu_set_t), &cpuSet);    
    }
    else{
        pthread_setaffinity_np(thread->native_handle(), sizeof(cpu_set_t), &cpuSet);
    }

    return newCoreId;
}
