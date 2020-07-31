#ifndef SOCIAL_NETWORK_MICROSERVICES_MYCLIENTPOOL_H
#define SOCIAL_NETWORK_MICROSERVICES_MYCLIENTPOOL_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <string>

#include "logger.h"

#include "MyThriftClient.h"

namespace my_social_network {

template<class MyTClient>
class MyClientPool {
 public:
  MyClientPool(const std::string &client_type, uint32_t buff_size,
               int min_size, int max_size, int timeout_ms);
  ~MyClientPool();

  MyClientPool(const MyClientPool&) = delete;
  MyClientPool& operator=(const MyClientPool&) = delete;
  MyClientPool(MyClientPool&&) = default;
  MyClientPool& operator=(MyClientPool&&) = default;

  MyTClient * Pop();
  void Push(MyTClient *);
  void Push(MyTClient *, int);
  void Remove(MyTClient *);

 private:
  std::deque<MyTClient *> _pool;
  // std::string _addr;
  std::string _client_type;
  // int _port;
  uint32_t _buff_size;
  int _min_pool_size{};
  int _max_pool_size{};
  int _curr_pool_size{};
  int _timeout_ms;
  std::mutex _mtx;
  std::condition_variable _cv;

};

template<class MyTClient>
MyClientPool<MyTClient>::MyClientPool(const std::string &client_type, uint32_t buff_size,
                                      int min_pool_size, int max_pool_size, int timeout_ms) {
  // _addr = addr;
  // _port = port;
  _buff_size = buff_size;
  _min_pool_size = min_pool_size;
  _max_pool_size = max_pool_size;
  _timeout_ms = timeout_ms;
  _client_type = client_type;

  for (int i = 0; i < min_pool_size; ++i) {
    MyTClient *client = new MyTClient(_buff_size);
    _pool.emplace_back(client);
  }
  _curr_pool_size = min_pool_size;
}

template<class MyTClient>
MyClientPool<MyTClient>::~MyClientPool() {
  while (!_pool.empty()) {
    delete _pool.front();
    _pool.pop_front();
  }
}

template<class MyTClient>
MyTClient * MyClientPool<MyTClient>::Pop() {
  MyTClient * client = nullptr;
  std::unique_lock<std::mutex> cv_lock(_mtx); {
    while (_pool.size() == 0) {
      // Create a new a client if current pool size is less than
      // the max pool size.
      if (_curr_pool_size < _max_pool_size) {
        try {
          client = new MyTClient(_buff_size);
          _curr_pool_size++;
          break;
        } catch (...) {
          cv_lock.unlock();
          return nullptr;
        }
      } else {
        auto wait_time = std::chrono::system_clock::now() +
            std::chrono::milliseconds(_timeout_ms);
        bool wait_success = _cv.wait_until(cv_lock, wait_time,
            [this] { return _pool.size() > 0; });
        if (!wait_success) {
          LOG(warning) << "MyClientPool pop timeout";
          cv_lock.unlock();
          return nullptr;
        }
      }
    }
    if (!client){
      client = _pool.front();
      _pool.pop_front();
    }

  } // cv_lock(_mtx)
  cv_lock.unlock();

  if (client) {
    try {
      client->Connect();
    } catch (...) {
      LOG(error) << "Failed to connect " + _client_type;
      _pool.push_back(client);
      throw;
    }    
  }
  return client;
}

template<class MyTClient>
void MyClientPool<MyTClient>::Push(MyTClient *client) {
  std::unique_lock<std::mutex> cv_lock(_mtx);
  client->KeepAlive();
  _pool.push_back(client);
  cv_lock.unlock();
  _cv.notify_one();
}

template<class MyTClient>
void MyClientPool<MyTClient>::Push(MyTClient *client, int timeout_ms) {
  std::unique_lock<std::mutex> cv_lock(_mtx);
  client->KeepAlive(timeout_ms);
  _pool.push_back(client);
  cv_lock.unlock();
  _cv.notify_one();
}

template<class MyTClient>
void MyClientPool<MyTClient>::Remove(MyTClient *client) {
  std::unique_lock<std::mutex> lock(_mtx);
  delete client;
  _curr_pool_size--;
  lock.unlock();
}

} // namespace social_network


#endif //SOCIAL_NETWORK_MICROSERVICES_MYCLIENTPOOL_H