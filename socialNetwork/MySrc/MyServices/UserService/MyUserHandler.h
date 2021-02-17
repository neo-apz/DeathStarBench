#ifndef SOCIAL_NETWORK_MICROSERVICES_USERHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_USERHANDLER_H_

#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <jwt/jwt.hpp>

#include <chrono>
#include <mutex>

#include <PicoSHA2/picosha2.h>

#include "../../gen-cpp/FakeMemcached.h"
#include "../../gen-cpp/FakeMongo.h"

#include "../../gen-cpp/UserService.h"
#include "../../gen-cpp/ComposePostService.h"
#include "../../gen-cpp/MySocialGraphService.h"

#include <logger.h>
#include <NebulaClientPool.h>
#include <MyThriftClient.h>

// Custom Epoch (January 1, 2018 Midnight GMT = 2018-01-01T00:00:00Z)
#define CUSTOM_EPOCH 1514764800000

namespace my_social_network {

using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;
using namespace jwt::params;

static int64_t current_timestamp = -1;
static int counter = 0;

static int GetCounter(int64_t timestamp) {
  if (current_timestamp > timestamp) {
    LOG(fatal) << "Timestamps are not incremental.";
    exit(EXIT_FAILURE);
  }
  if (current_timestamp == timestamp) {
    return counter++;
  } else {
    current_timestamp = timestamp;
    counter = 0;
    return counter++;
  }
}

std::string GenRandomString(const int len) {
  static const std::string alphanum =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(
      0, static_cast<int>(alphanum.length() - 1));
  std::string s;
  for (int i = 0; i < len; ++i) {
    s += alphanum[dist(gen)];
  }
  return s;
}


class UserHandler : public UserServiceIf {
 public:
  UserHandler(
      std::mutex*,
      const std::string &,
      const std::string &,
			NebulaClientPool<FakeMemcachedClient> *,
			NebulaClientPool<FakeMongoClient> *,
			NebulaClientPool<MySocialGraphServiceClient> *,
			NebulaClientPool<ComposePostServiceClient> *
		);
  ~UserHandler() override = default;

  bool RegisterUser(
      int64_t,
      const std::string &,
      const std::string &,
      const std::string &,
      const std::string &) override;
  bool RegisterUserWithId(
      int64_t,
      const std::string &,
      const std::string &,
      const std::string &,
      const std::string &,
      int64_t) override;

  void UploadCreatorWithUserId(
      int64_t,
      int64_t,
      const std::string &) override;
  bool UploadCreatorWithUsername(
      int64_t,
      const std::string &) override;
  void Login(
      std::string &,
      int64_t,
      const std::string &,
      const std::string &) override;
  int64_t GetUserId(
      int64_t,
      const std::string &) override;

 	NebulaClientPool<FakeMemcachedClient> *_memcached_pool;
	NebulaClientPool<FakeMongoClient> *_mongo_pool;
	NebulaClientPool<MySocialGraphServiceClient> *_socialgraph_pool;
	NebulaClientPool<ComposePostServiceClient> *_compose_pool;
 
 private:
  std::string _machine_id;
  std::string _secret;
  std::mutex *_thread_lock;
  
};

UserHandler::UserHandler(std::mutex *thread_lock,
														const std::string &machine_id,
														const std::string &secret,
														NebulaClientPool<FakeMemcachedClient> *memcached_pool,
														NebulaClientPool<FakeMongoClient> *mongo_pool,
														NebulaClientPool<MySocialGraphServiceClient> *socialgraph_pool,
														NebulaClientPool<ComposePostServiceClient> *compose_pool) {
  
  _thread_lock = thread_lock;
  _machine_id = machine_id;
  _secret = secret;

	_memcached_pool = memcached_pool;
	_mongo_pool = mongo_pool;
	_socialgraph_pool = socialgraph_pool;
	_compose_pool = compose_pool;
}

bool UserHandler::RegisterUserWithId(
    const int64_t req_id,
    const std::string &first_name,
    const std::string &last_name,
    const std::string &username,
    const std::string &password,
    const int64_t user_id) {

	
	bool found;
	
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo to check if user already exists.
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::USER_EXISTS);
		found = mongo_client->UserExists(username);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::USER_EXISTS);
		auto mongo_client = mongo_client_wrapper->GetClient();
		found = mongo_client->UserExists(username);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (found) {
		// LOG(warning) << "User " << username << " already existed.";
		return false;
	}

	User user;
	user.user_id = user_id;
	user.first_name = first_name;
	user.last_name = last_name;
	user.username = username;
	
	std::string salt = GenRandomString(32);
	user.salt = salt;

	user.password_hashed = picosha2::hash256_hex_string(password + salt);

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo to insert user.
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::INSERT_USER);
		mongo_client->InsertUser(user);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::INSERT_USER);
		auto mongo_client = mongo_client_wrapper->GetClient();
		mongo_client->InsertUser(user);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							<< e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif		

	// LOG(debug) << "User: " << username << " registered";

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to SocialGraph to insert user.
	try {
		#ifdef CEREBROS
		auto socialgraph_client = _socialgraph_pool->Get(MySocialGraphServiceIf::FuncType::INSERT_USER);
		socialgraph_client->InsertUser(req_id, user_id);
		#else
		auto socialgraph_client_wrapper = _socialgraph_pool->Get(MySocialGraphServiceIf::FuncType::INSERT_USER);
		auto socialgraph_client = socialgraph_client_wrapper->GetClient();
		socialgraph_client->InsertUser(req_id, user_id);
		socialgraph_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to SocialGraph server:\n"
							<< e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif		

	return true;

}

bool UserHandler::RegisterUser(
    const int64_t req_id,
    const std::string &first_name,
    const std::string &last_name,
    const std::string &username,
    const std::string &password) {

  // Compose user_id
  _thread_lock->lock();
  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count() - CUSTOM_EPOCH;
  int idx = GetCounter(timestamp);
  _thread_lock->unlock();

  std::stringstream sstream;
  sstream << std::hex << timestamp;
  std::string timestamp_hex(sstream.str());
  if (timestamp_hex.size() > 10) {
    timestamp_hex.erase(0, timestamp_hex.size() - 10);
  } else if (timestamp_hex.size() < 10) {
    timestamp_hex =
        std::string(10 - timestamp_hex.size(), '0') + timestamp_hex;
  }
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
  std::string user_id_str = _machine_id + timestamp_hex + counter_hex;
  int64_t user_id = stoul(user_id_str, nullptr, 16) & 0x7FFFFFFFFFFFFFFF;
	LOG(debug) << "The user_id of the request " << req_id << " is " << user_id;

	bool found;
	
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo to check if user already exists.
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::USER_EXISTS);
		found = mongo_client->UserExists(username);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::USER_EXISTS);
		auto mongo_client = mongo_client_wrapper->GetClient();
		found = mongo_client->UserExists(username);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (found) {
		// LOG(warning) << "User " << username << " already existed.";
		return false;
	}

	User user;
	user.user_id = user_id;
	user.first_name = first_name;
	user.last_name = last_name;
	user.username = username;
	
	std::string salt = GenRandomString(32);
	user.salt = salt;

	user.password_hashed = picosha2::hash256_hex_string(password + salt);

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo to insert user.
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::INSERT_USER);
		mongo_client->InsertUser(user);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::INSERT_USER);
		auto mongo_client = mongo_client_wrapper->GetClient();
		mongo_client->InsertUser(user);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							<< e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif		

	// LOG(debug) << "User: " << username << " registered";

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to SocialGraph to insert user.
	try {
		#ifdef CEREBROS
		auto socialgraph_client = _socialgraph_pool->Get(MySocialGraphServiceIf::FuncType::INSERT_USER);
		socialgraph_client->InsertUser(req_id, user_id);
		#else
		auto socialgraph_client_wrapper = _socialgraph_pool->Get(MySocialGraphServiceIf::FuncType::INSERT_USER);
		auto socialgraph_client = socialgraph_client_wrapper->GetClient();
		socialgraph_client->InsertUser(req_id, user_id);
		socialgraph_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to SocialGraph server:\n"
							<< e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif		

	return true;
}

bool UserHandler::UploadCreatorWithUsername(
    const int64_t req_id,
    const std::string &username) {

	bool cached = false;
	int64_t user_id = -1;

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMemcached to check if user is cached.
	try {
		#ifdef CEREBROS
		auto memcached_client = _memcached_pool->Get(FakeMemcachedIf::FuncType::USER_CACHED);
		user_id = memcached_client->UserCached(username);
		#else
		auto memcached_client_wrapper = _memcached_pool->Get(FakeMemcachedIf::FuncType::USER_CACHED);
		auto memcached_client = memcached_client_wrapper->GetClient();
		user_id = memcached_client->UserCached(username);
		memcached_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Memcached server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (user_id != -1){ // Cached!
		cached = true;
		// LOG(debug) << "Found user_id of username :" << username  << " in Memcached";
	}
	else { // If not cached in memcached
		// LOG(debug) << "user_id not cached in Memcached";

		#ifdef __aarch64__
			NESTED_BEGIN();
			NESTED_DISPATCH_BEGIN();
		#endif
		// Connect to FakeMongo to check if user exists.
		try {
			#ifdef CEREBROS
			auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::CREATOR_EXISTS);
			user_id = mongo_client->CreatorExists(username);
			#else
			auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::CREATOR_EXISTS);
			auto mongo_client = mongo_client_wrapper->GetClient();
			user_id = mongo_client->CreatorExists(username);
			mongo_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Mongo server:\n"
								<< e.what() << '\n' ;
			exit(EXIT_FAILURE);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif		

		if (user_id == -1) {
			// LOG(warning) << "User: " << username << " doesn't exist in MongoDB";
			return false;
		}

		// LOG(debug) << "User: " << username << " found in MongoDB";
	}

	Creator creator;
  creator.username = username;
  creator.user_id = user_id;

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Upload to compose post service
	try {
		#ifdef CEREBROS
		auto compose_post_client = _compose_pool->Get(ComposePostServiceIf::FuncType::UPLOAD_CREATOR);
		compose_post_client->UploadCreator(req_id, creator);
		#else
		auto compose_post_client_wrapper = _compose_pool->Get(ComposePostServiceIf::FuncType::UPLOAD_CREATOR);
		auto compose_post_client = compose_post_client_wrapper->GetClient();
		compose_post_client->UploadCreator(req_id, creator);
		compose_post_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to upload creator to compose-post-service:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (!cached) {
		#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
		// Connect to FakeMemcached to insert user_id.
		try {
			#ifdef CEREBROS
			auto memcached_client = _memcached_pool->Get(FakeMemcachedIf::FuncType::INSERT_USER_ID);
			memcached_client->InsertUserId(username, user_id);
			#else
			auto memcached_client_wrapper = _memcached_pool->Get(FakeMemcachedIf::FuncType::INSERT_USER_ID);
			auto memcached_client = memcached_client_wrapper->GetClient();
			memcached_client->InsertUserId(username, user_id);
			memcached_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Memcached server:\n"
								<< e.what() << '\n' ;
			exit(EXIT_FAILURE);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif
	}

	return true;
}

void UserHandler::UploadCreatorWithUserId(
    int64_t req_id,
    int64_t user_id,
    const std::string &username) {

	Creator creator;
  creator.username = username;
  creator.user_id = user_id;

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Upload to compose post service
	try {
		#ifdef CEREBROS
		auto compose_post_client = _compose_pool->Get(ComposePostServiceIf::FuncType::UPLOAD_CREATOR);
		compose_post_client->UploadCreator(req_id, creator);
		#else
		auto compose_post_client_wrapper = _compose_pool->Get(ComposePostServiceIf::FuncType::UPLOAD_CREATOR);
		auto compose_post_client = compose_post_client_wrapper->GetClient();
		compose_post_client->UploadCreator(req_id, creator);
		compose_post_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to upload creator to compose-post-service:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
	#ifdef __aarch64__
		NESTED_END();
	#endif

}

void UserHandler::Login(
    std::string & _return,
    int64_t req_id,
    const std::string &username,
    const std::string &password) {

  User user;
	
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMemcached to check if login is cached.
	try {
		#ifdef CEREBROS
		auto memcached_client = _memcached_pool->Get(FakeMemcachedIf::FuncType::LOGIN_CACHED);
		memcached_client->LoginCached(user, username);
		#else
		auto memcached_client_wrapper = _memcached_pool->Get(FakeMemcachedIf::FuncType::LOGIN_CACHED);
		auto memcached_client = memcached_client_wrapper->GetClient();
		memcached_client->LoginCached(user, username);
		memcached_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Memcached server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	bool cached = false;

	if (user.user_id != -1) { // Login is cached
		cached = true;
		// LOG(debug) << "Found username: "<< username <<" in Memcached";
	}

	else {  // TODO: If not cached in memcached

	}

	bool auth = picosha2::hash256_hex_string(password + user.salt) == user.password_hashed;

	if (auth) {
		auto user_id_str = std::to_string(user.user_id);
      auto timestamp_str = std::to_string(duration_cast<seconds>(
          system_clock::now().time_since_epoch()).count());
      jwt::jwt_object obj{
          algorithm("HS256"),
          secret(_secret),
          payload({
              {"user_id", user_id_str},
              {"username", user.username},
              {"timestamp", timestamp_str},
              {"ttl", "3600"}
          })
      };
      _return = obj.signature();
	}
	else {
		_return = "ERROR: Incorrect username or password";
		return;
	}

	if (!cached) {
		#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
		// Connect to FakeMemcached to insert login info
		try {
			#ifdef CEREBROS
			auto memcached_client = _memcached_pool->Get(FakeMemcachedIf::FuncType::INSERT_USER);
			memcached_client->InsertUser(username, user);
			#else
			auto memcached_client_wrapper = _memcached_pool->Get(FakeMemcachedIf::FuncType::INSERT_USER);
			auto memcached_client = memcached_client_wrapper->GetClient();
			memcached_client->InsertUser(username, user);
			memcached_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Memcached server:\n"
								<< e.what() << '\n' ;
			exit(EXIT_FAILURE);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif		
	}

}

int64_t UserHandler::GetUserId(
    int64_t req_id,
    const std::string &username) {

	bool cached = false;
	int64_t user_id = -1;

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMemcached to check if user is cached.
	try {
		#ifdef CEREBROS
		auto memcached_client = _memcached_pool->Get(FakeMemcachedIf::FuncType::GET_USER_ID);
		user_id = memcached_client->GetUserId(username);
		#else
		auto memcached_client_wrapper = _memcached_pool->Get(FakeMemcachedIf::FuncType::GET_USER_ID);
		auto memcached_client = memcached_client_wrapper->GetClient();
		user_id = memcached_client->GetUserId(username);
		memcached_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Memcached server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	User user;

	if (user_id != -1){ // Cached!
		cached = true;
		// LOG(debug) << "Found user_id of username :" << username  << " in Memcached";
	}
	else { // If not cached in memcached
		// LOG(debug) << "user_id not cached in Memcached";

		#ifdef __aarch64__
			NESTED_BEGIN();
			NESTED_DISPATCH_BEGIN();
		#endif
		// Connect to FakeMongo to check if user exists.
		try {
			#ifdef CEREBROS
			auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::GET_USER);
			mongo_client->GetUser(user, username);
			#else
			auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::GET_USER);
			auto mongo_client = mongo_client_wrapper->GetClient();
			mongo_client->GetUser(user, username);
			mongo_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Mongo server:\n"
								<< e.what() << '\n' ;
			exit(EXIT_FAILURE);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif		

		user_id = user.user_id;

		if (user_id == -1) {
			// LOG(warning) << "User: " << username << " doesn't exist in MongoDB";
			return user_id;
		}

		// LOG(debug) << "User: " << username << " found in MongoDB";
	}

	if (!cached) {
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
		// Connect to FakeMemcached to insert user_id.
		try {
			#ifdef CEREBROS
			auto memcached_client = _memcached_pool->Get(FakeMemcachedIf::FuncType::INSERT_USER_ID);
			memcached_client->InsertUserId(username, user_id);
			#else
			auto memcached_client_wrapper = _memcached_pool->Get(FakeMemcachedIf::FuncType::INSERT_USER_ID);
			auto memcached_client = memcached_client_wrapper->GetClient();
			memcached_client->InsertUserId(username, user_id);
			memcached_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Memcached server:\n"
								<< e.what() << '\n' ;
			exit(EXIT_FAILURE);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif
	}

	return user_id;

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

#endif //SOCIAL_NETWORK_MICROSERVICES_USERHANDLER_H_
