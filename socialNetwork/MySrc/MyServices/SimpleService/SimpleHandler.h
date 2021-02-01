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


#include "../../gen-cpp/UniqueIdService.h"
#include "../../gen-cpp/my_social_network_types.h"

#include "../../gen-cpp/ComposePostService.h"

#include <logger.h>
#include <NebulaClientPool.h>
#include <MyThriftClient.h>

// Custom Epoch (January 1, 2018 Midnight GMT = 2018-01-01T00:00:00Z)
#define CUSTOM_EPOCH 1514764800000

namespace my_social_network {

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

static int64_t current_timestamp = -1;
static int counter = 0;

static int GetCounter(int64_t timestamp) {
  if (current_timestamp == timestamp) {
    return counter++;
  } else {
    current_timestamp = timestamp;
    counter = 0;
    return counter++;
  }
}

class UniqueIdHandler : public UniqueIdServiceIf {
 public:
  ~UniqueIdHandler() override = default;
  UniqueIdHandler(
      std::mutex *,
      const std::string &,
			NebulaClientPool<ComposePostServiceClient> *);

  int64_t UploadUniqueId(int64_t, PostType::type) override;

 private:
  std::mutex *_thread_lock;
  // MyLock *_thread_lock;
  std::string _machine_id;
	NebulaClientPool<ComposePostServiceClient> *_compose_client_pool;
};

UniqueIdHandler::UniqueIdHandler(
    std::mutex *thread_lock,
    const std::string &machine_id,
		NebulaClientPool<ComposePostServiceClient> * compose_client_pool){
  _thread_lock = thread_lock;
  _machine_id = machine_id;
	_compose_client_pool = compose_client_pool;
}

int64_t UniqueIdHandler::UploadUniqueId(
    int64_t req_id,
    PostType::type post_type) {

  int64_t timestamp = CUSTOM_EPOCH + req_id;
  int idx = GetCounter(timestamp);

  int64_t post_id = idx & 0x7FFFFFFFFFFFFFFF;

	#ifdef __aarch64__
		NESTED_BEGIN();
	#endif
	// Upload to compose post service
	try {
		#ifdef CEREBROS
		auto compose_post_client = _compose_client_pool->Get(ComposePostServiceIf::FuncType::UPLOAD_UNIQUE_ID);
		compose_post_client->UploadUniqueId(req_id, post_id, post_type);
		#else
		auto compose_post_client_wrapper = _compose_client_pool->Get(ComposePostServiceIf::FuncType::UPLOAD_UNIQUE_ID);
		auto compose_post_client = compose_post_client_wrapper->GetClient();
		compose_post_client->UploadUniqueId(req_id, post_id, post_type);
		compose_post_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to upload unique-id to compose-post-service:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
	#ifdef __aarch64__
		NESTED_END();
	#endif

  return post_id;
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

} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_MYUNIQUEIDHANDLER_H
