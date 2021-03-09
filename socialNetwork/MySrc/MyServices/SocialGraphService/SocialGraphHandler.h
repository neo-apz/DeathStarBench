#ifndef SOCIAL_NETWORK_MICROSERVICES_SOCIALGRAPHHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_SOCIALGRAPHHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

#include "../../gen-cpp/MySocialGraphService.h"

#include "../../gen-cpp/FakeMongo.h"
#include "../../gen-cpp/FakeRedis.h"
#include "../../gen-cpp/UserService.h"

#include <logger.h>
#include <NebulaClientPool.h>
#include <MyThriftClient.h>

namespace my_social_network {
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

class SocialGraphHandler : public MySocialGraphServiceIf {
 public:
  SocialGraphHandler(
		NebulaClientPool<FakeMongoClient> *mongo_pool,
		NebulaClientPool<FakeRedisClient> *redis_pool,
		NebulaClientPool<UserServiceClient> *user_pool);
  ~SocialGraphHandler() override = default;

  void GetFollowers(std::vector<int64_t> &, int64_t, int64_t) override;
  void GetFollowees(std::vector<int64_t> &, int64_t, int64_t) override;
  void Follow(int64_t, int64_t, int64_t) override;
  void Unfollow(int64_t, int64_t, int64_t) override;
  void FollowWithUsername(int64_t, const std::string &, const std::string &) override;
  void UnfollowWithUsername(int64_t, const std::string &, const std::string &) override;
  void InsertUser(int64_t, int64_t) override;

	NebulaClientPool<FakeMongoClient> *_mongo_pool;
 	NebulaClientPool<FakeRedisClient> *_redis_pool;
	NebulaClientPool<UserServiceClient> *_user_pool;

};

SocialGraphHandler::SocialGraphHandler(
		NebulaClientPool<FakeMongoClient> *mongo_pool,
		NebulaClientPool<FakeRedisClient> *redis_pool,
		NebulaClientPool<UserServiceClient> *user_pool) {

	_redis_pool = redis_pool;
	_user_pool = user_pool;
	_mongo_pool = mongo_pool;
}

void SocialGraphHandler::GetFollowers(std::vector<int64_t> & _return,
                           const int64_t req_id, const int64_t user_id) {

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::GET_FLWRS);
		redis_client->GetFLWRs(_return, user_id, "followers");
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::GET_FLWRS);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->GetFLWRs(_return, user_id, "followers");
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (_return.size() == 0) { // Was not found in Redis!
		#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
		#endif
		// Connect to FakeMongo 
		try {
			#ifdef CEREBROS
			auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::GET_FLWRS);
			mongo_client->GetFLWRs(_return, user_id);
			#else
			auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::GET_FLWRS);
			auto mongo_client = mongo_client_wrapper->GetClient();
			mongo_client->GetFLWRs(_return, user_id);
			mongo_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Mongo server:\n"
								<< e.what() << '\n' ;
			#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif

		// Insert fetched data into Redis.
		#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
		#endif
		// Connect to FakeRedis
		try {
			#ifdef CEREBROS
			auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWRS);
			redis_client->PutFLWRs(user_id, "followers", _return);
			#else
			auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWRS);
			auto redis_client = redis_client_wrapper->GetClient();
			redis_client->PutFLWRs(user_id, "followers", _return);
			redis_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Redis server:\n"
								<< e.what() << '\n' ;
			#ifdef __aarch64__
			NOTIFY_EXCEPTION(0);
			#endif

			syscall(SYS_exit_group, 0);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif
	}
}

void SocialGraphHandler::GetFollowees(std::vector<int64_t> & _return,
                           const int64_t req_id, const int64_t user_id) {

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::GET_FLWEES);
		redis_client->GetFLWEEs(_return, user_id, "followees");
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::GET_FLWEES);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->GetFLWEEs(_return, user_id, "followees");
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (_return.size() == 0) { // Was not found in Redis!
		#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
		#endif
		// Connect to FakeMongo 
		try {
			#ifdef CEREBROS
			auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::GET_FLWEES);
			mongo_client->GetFLWEEs(_return, user_id);
			#else
			auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::GET_FLWEES);
			auto mongo_client = mongo_client_wrapper->GetClient();
			mongo_client->GetFLWEEs(_return, user_id);
			mongo_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Mongo server:\n"
								<< e.what() << '\n' ;
			#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif

		// Insert fetched data into Redis
		#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
		#endif
		// Connect to FakeRedis
		try {
			#ifdef CEREBROS
			auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWEES);
			redis_client->PutFLWEEs(user_id, "followees", _return);
			#else
			auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWEES);
			auto redis_client = redis_client_wrapper->GetClient();
			redis_client->PutFLWEEs(user_id, "followees", _return);
			redis_client_wrapper->ResetBuffers(true, false);
			#endif
		} catch(const std::exception& e) {
			LOG(error) << "Cannot connect to Redis server:\n"
								<< e.what() << '\n' ;
			#ifdef __aarch64__
			NOTIFY_EXCEPTION(0);
			#endif

			syscall(SYS_exit_group, 0);
		}

		#ifdef __aarch64__
			NESTED_END();
		#endif
	}
}

void SocialGraphHandler::Follow(
    int64_t req_id,
    int64_t user_id,
    int64_t followee_id) {

  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count();

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo 
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::ADD_FLWR);
		mongo_client->AddFollower(user_id, followee_id, timestamp);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::ADD_FLWR);
		auto mongo_client = mongo_client_wrapper->GetClient();
		mongo_client->AddFollower(user_id, followee_id, timestamp);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif


	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo 
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::ADD_FLWEE);
		mongo_client->AddFollowee(followee_id, user_id, timestamp);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::ADD_FLWEE);
		auto mongo_client = mongo_client_wrapper->GetClient();
		mongo_client->AddFollowee(followee_id, user_id, timestamp);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWR);
		redis_client->PutFLWR(user_id, "followers", followee_id, timestamp);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWR);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->PutFLWR(user_id, "followers", followee_id, timestamp);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif


	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWEE);
		redis_client->PutFLWEE(followee_id, "followees", user_id, timestamp);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::PUT_FLWEE);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->PutFLWEE(followee_id, "followees", user_id, timestamp);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif
}


void SocialGraphHandler::Unfollow(
    int64_t req_id,
    int64_t user_id,
    int64_t followee_id) {

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo 
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::RMV_FLWR);
		mongo_client->RemoveFollower(user_id, followee_id);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::RMV_FLWR);
		auto mongo_client = mongo_client_wrapper->GetClient();
		mongo_client->RemoveFollower(user_id, followee_id);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif


	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo 
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::RMV_FLWEE);
		mongo_client->RemoveFollowee(followee_id, user_id);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::RMV_FLWEE);
		auto mongo_client = mongo_client_wrapper->GetClient();
		mongo_client->RemoveFollowee(followee_id, user_id);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::RMV_FLWR);
		redis_client->RemoveFLWR(user_id, "followers", followee_id);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::RMV_FLWR);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->RemoveFLWR(user_id, "followers", followee_id);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif


	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::RMV_FLWEE);
		redis_client->RemoveFLWEE(followee_id, "followees", user_id);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::RMV_FLWEE);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->RemoveFLWEE(followee_id, "followees", user_id);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

}

void SocialGraphHandler::FollowWithUsername(
    int64_t req_id,
    const std::string &user_name,
    const std::string &followee_name) {

  int64_t user_id = -1;
  int64_t followee_id = -1;

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to User to get the user ID.
	try {
		#ifdef CEREBROS
		auto user_client = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		user_id = user_client->GetUserId(req_id, user_name);
		#else
		auto user_client_wrapper = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		auto user_client = user_client_wrapper->GetClient();
		user_id = user_client->GetUserId(req_id, user_name);
		user_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to User server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to User to get the user ID.
	try {
		#ifdef CEREBROS
		auto user_client = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		followee_id = user_client->GetUserId(req_id, followee_name);
		#else
		auto user_client_wrapper = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		auto user_client = user_client_wrapper->GetClient();
		followee_id = user_client->GetUserId(req_id, followee_name);
		user_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to User server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (user_id != -1 && followee_id != -1)
		Follow(req_id, user_id, followee_id);
}

void SocialGraphHandler::UnfollowWithUsername(
    int64_t req_id,
    const std::string &user_name,
    const std::string &followee_name) {

  int64_t user_id = -1;
  int64_t followee_id = -1;

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to User to get the user ID.
	try {
		#ifdef CEREBROS
		auto user_client = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		user_id = user_client->GetUserId(req_id, user_name);
		#else
		auto user_client_wrapper = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		auto user_client = user_client_wrapper->GetClient();
		user_id = user_client->GetUserId(req_id, user_name);
		user_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to User server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to User to get the user ID.
	try {
		#ifdef CEREBROS
		auto user_client = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		followee_id = user_client->GetUserId(req_id, followee_name);
		#else
		auto user_client_wrapper = _user_pool->Get(UserServiceIf::FuncType::GET_UID);
		auto user_client = user_client_wrapper->GetClient();
		followee_id = user_client->GetUserId(req_id, followee_name);
		user_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to User server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

	if (user_id != -1 && followee_id != -1)
		Unfollow(req_id, user_id, followee_id);
}

void SocialGraphHandler::InsertUser(int64_t req_id, int64_t user_id) {
	#ifdef __aarch64__
	NESTED_BEGIN();
	NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeMongo 
	try {
		#ifdef CEREBROS
		auto mongo_client = _mongo_pool->Get(FakeMongoIf::FuncType::INSERT_USERID);
		mongo_client->InsertUserId(user_id);
		#else
		auto mongo_client_wrapper = _mongo_pool->Get(FakeMongoIf::FuncType::INSERT_USERID);
		auto mongo_client = mongo_client_wrapper->GetClient();
		mongo_client->InsertUserId(user_id);
		mongo_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Mongo server:\n"
							<< e.what() << '\n' ;
		#ifdef __aarch64__
		NOTIFY_EXCEPTION(0);
		#endif

		syscall(SYS_exit_group, 0);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

}

} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_SOCIALGRAPHHANDLER_H_
