#ifndef SOCIAL_NETWORK_MICROSERVICES_MYLOCK_H
#define SOCIAL_NETWORK_MICROSERVICES_MYLOCK_H

#include <atomic>

class MyLock {
public:
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};

#endif // #ifndef SOCIAL_NETWORK_MICROSERVICES_MYLOCK_H