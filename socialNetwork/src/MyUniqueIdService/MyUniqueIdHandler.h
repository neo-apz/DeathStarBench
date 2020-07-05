#ifndef SOCIAL_NETWORK_MICROSERVICES_MYUNIQUEIDHANDLER_H
#define SOCIAL_NETWORK_MICROSERVICES_MYUNIQUEIDHANDLER_H

#include <iostream>
#include <string>
#include <chrono>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>


#include "../../gen-cpp/MyUniqueIdService.h"
#include "../../gen-cpp/MyComposePostService.h"
#include "../../gen-cpp/social_network_types.h"

// #include "../ClientPool.h"
#include "../MyCommon/MyClientPool.h"
// #include "../MyCommon/MyThriftClient.h"

#include "../logger.h"
// #include "../tracing.h"

// Custom Epoch (January 1, 2018 Midnight GMT = 2018-01-01T00:00:00Z)
#define CUSTOM_EPOCH 1514764800000

namespace social_network {

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

static int64_t current_timestamp = -1;
static int counter = 0;

static int GetCounter(int64_t timestamp) {
  if (current_timestamp > timestamp) {
    // LOG(fatal) << "Timestamps are not incremental.";
    // std::cout << " Current timestamp: " << current_timestamp << " timestamp: " << timestamp << std::endl;
    // exit(EXIT_FAILURE);
    return counter++;
  }
  if (current_timestamp == timestamp) {
    return counter++;
  } else {
    current_timestamp = timestamp;
    counter = 0;
    return counter++;
  }
}

class MyUniqueIdHandler : public MyUniqueIdServiceIf {
 public:
  ~MyUniqueIdHandler() override = default;
  MyUniqueIdHandler(
      std::mutex *,
      const std::string &,
      MyClientPool<MyThriftClient<MyComposePostServiceClient>> *);
      // std::shared_ptr<MyComposePostServiceProcessor>);

  void UploadUniqueId(int64_t, PostType::type) override;

 private:
  std::mutex *_thread_lock;
  std::string _machine_id;
  MyClientPool<MyThriftClient<MyComposePostServiceClient>> *_compose_client_pool;
  // MyThriftClient<MyComposePostServiceClient> *_compose_client;
  // std::shared_ptr<MyComposePostServiceProcessor> _compose_processor;
};

MyUniqueIdHandler::MyUniqueIdHandler(
    std::mutex *thread_lock,
    const std::string &machine_id,
    MyClientPool<MyThriftClient<MyComposePostServiceClient>> *compose_client_pool){
    // std::shared_ptr<MyComposePostServiceProcessor> compose_processor) {
  _thread_lock = thread_lock;
  _machine_id = machine_id;
  // _compose_clients = compose_clients;
  _compose_client_pool = compose_client_pool;
  // _compose_processor = compose_processor;
}

void MyUniqueIdHandler::UploadUniqueId(
    int64_t req_id,
    PostType::type post_type) {

  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count() - CUSTOM_EPOCH;
  std::stringstream sstream;
  sstream << std::hex << timestamp;
  std::string timestamp_hex(sstream.str());

  if (timestamp_hex.size() > 10) {
    timestamp_hex.erase(0, timestamp_hex.size() - 10);
  } else if (timestamp_hex.size() < 10) {
    timestamp_hex = std::string(10 - timestamp_hex.size(), '0') + timestamp_hex;
  }

  _thread_lock->lock();  
  int idx = GetCounter(timestamp);
  _thread_lock->unlock();

  // Empty the sstream buffer.
  sstream.clear();
  sstream.str(std::string());

  sstream << std::hex << idx;
  std::string counter_hex(sstream.str());

  if (counter_hex.size() > 3) {
    counter_hex.erase(0, counter_hex.size() - 3);
  } else if (counter_hex.size() < 3) {
    counter_hex = std::string(3 - counter_hex.size(), '0') + counter_hex;
  }
  std::string post_id_str = _machine_id + timestamp_hex + counter_hex;
  int64_t post_id = stoul(post_id_str, nullptr, 16) & 0x7FFFFFFFFFFFFFFF;
  LOG(debug) << "The post_id of the request "
      << req_id << " is " << post_id;

  // Upload to compose post service
  auto compose_post_client_wrapper = _compose_client_pool->Pop();
  if (!compose_post_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
    se.message = "Failed to connect to compose-post-service";
    throw se;
  }
  auto compose_post_client = compose_post_client_wrapper->GetClient();
  try {
    compose_post_client->UploadUniqueId(req_id, post_id, post_type);    
  } catch (...) {
    _compose_client_pool->Push(compose_post_client_wrapper);
    LOG(error) << "Failed to upload unique-id to compose-post-service";
    throw;
  }

  _compose_client_pool->Push(compose_post_client_wrapper);
}

/*
 * The following code which obtaines machine ID from machine's MAC address was
 * inspired from https://stackoverflow.com/a/16859693.
 */
u_int16_t HashMacAddressPid(const std::string &mac)
{
  u_int16_t hash = 0;
  std::string mac_pid = mac + std::to_string(getpid());
  for ( unsigned int i = 0; i < mac_pid.size(); i++ ) {
    hash += ( mac[i] << (( i & 1 ) * 8 ));
  }
  return hash;
}

int GetMachineId (std::string *mac_hash) {
  std::string mac;
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP );
  if ( sock < 0 ) {
    LOG(error) << "Unable to obtain MAC address";
    return -1;
  }

  struct ifconf conf{};
  char ifconfbuf[ 128 * sizeof(struct ifreq)  ];
  memset( ifconfbuf, 0, sizeof( ifconfbuf ));
  conf.ifc_buf = ifconfbuf;
  conf.ifc_len = sizeof( ifconfbuf );
  if ( ioctl( sock, SIOCGIFCONF, &conf ))
  {
    LOG(error) << "Unable to obtain MAC address";
    return -1;
  }

  struct ifreq* ifr;
  for (
      ifr = conf.ifc_req;
      reinterpret_cast<char *>(ifr) <
          reinterpret_cast<char *>(conf.ifc_req) + conf.ifc_len;
      ifr++) {
    if ( ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data ) {
      continue;  // duplicate, skip it
    }

    if ( ioctl( sock, SIOCGIFFLAGS, ifr )) {
      continue;  // failed to get flags, skip it
    }
    if ( ioctl( sock, SIOCGIFHWADDR, ifr ) == 0 ) {
      mac = std::string(ifr->ifr_addr.sa_data);
      if (!mac.empty()) {
        break;
      }
    }
  }
  close(sock);

  std::stringstream stream;
  stream << std::hex << HashMacAddressPid(mac);
  *mac_hash = stream.str();

  if (mac_hash->size() > 3) {
    mac_hash->erase(0, mac_hash->size() - 3);
  } else if (mac_hash->size() < 3) {
    *mac_hash = std::string(3 - mac_hash->size(), '0') + *mac_hash;
  }
  return 0;
}

} // namespace social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_MYUNIQUEIDHANDLER_H
