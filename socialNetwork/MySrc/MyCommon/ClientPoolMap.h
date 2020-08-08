#ifndef SOCIAL_NETWORK_MICROSERVICES_CLIENTPOLLMAP_H
#define SOCIAL_NETWORK_MICROSERVICES_CLIENTPOLLMAP_H

#include <vector>
#include <mutex>
#include <map>
#include <vector>
#include <string>

#include "logger.h"

#include "MyThriftClient.h"

namespace my_social_network {

template<class MyTClient>
class ClientPoolMap {
 public:
  ClientPoolMap(const std::string &client_type, uint32_t buff_size, int pool_size);
  ~ClientPoolMap();

  ClientPoolMap(const ClientPoolMap&) = delete;
  ClientPoolMap& operator=(const ClientPoolMap&) = delete;
  ClientPoolMap(ClientPoolMap&&) = default;
  ClientPoolMap& operator=(ClientPoolMap&&) = default;

  MyTClient * Pop();
  void Push(MyTClient *);

 private:
  std::vector<MyTClient *> _pool;
  std::map<std::thread::id, int> _index_map;
  int _current_index;
  std::string _client_type;
  uint32_t _buff_size;
  int _pool_size{};
  std::mutex _mtx;
};

template<class MyTClient>
ClientPoolMap<MyTClient>::ClientPoolMap(const std::string &client_type,
                                        uint32_t buff_size, int pool_size) {
  _buff_size = buff_size;
  _pool_size = pool_size;
  _client_type = client_type;

  for (int i = 0; i < _pool_size; ++i) {
    MyTClient *client = new MyTClient(_buff_size);
    _pool.emplace_back(client);
  }
  _current_index = 0;
}

template<class MyTClient>
ClientPoolMap<MyTClient>::~ClientPoolMap() {
  for (auto element : _pool) {
      delete element;
  }
}

template<class MyTClient>
MyTClient * ClientPoolMap<MyTClient>::Pop() {
  std::thread::id tid =  std::this_thread::get_id();

  int index = -1;

  _mtx.lock();

  if (_index_map.find(tid) == _index_map.end()) { // first time!
    _index_map[tid] = _current_index;
    index = _current_index++;
  }

  index = _index_map[tid];

  if (index >= _pool_size){
    LOG(error) << "Index is bigger than buffer size.";
    exit(1);
  }

  _mtx.unlock();

  MyTClient * client = _pool[index];

  if (!client)
    LOG(error) << "Client is NULL!";

  try {
    client->Connect();
  } catch (...) {
    LOG(error) << "Failed to connect " + _client_type;
    throw;
  }

  return client;
}

template<class MyTClient>
void ClientPoolMap<MyTClient>::Push(MyTClient *client) {

}

} // namespace my_social_network


#endif //SOCIAL_NETWORK_MICROSERVICES_CLIENTPOLLMAP_H