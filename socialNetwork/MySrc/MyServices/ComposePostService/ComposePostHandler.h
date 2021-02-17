#ifndef SOCIAL_NETWORK_MICROSERVICES_COMPOSEPOSTHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_COMPOSEPOSTHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <chrono>


#include "../../gen-cpp/ComposePostService.h"

#include "../../gen-cpp/FakeRedis.h"
#include "../../gen-cpp/FakeRabbitmq.h"
#include "../../gen-cpp/PostStorageService.h"
#include "../../gen-cpp/UserTimelineService.h"

#include <logger.h>
#include <NebulaClientPool.h>
#include <MyThriftClient.h>

#define NUM_COMPONENTS 6

namespace my_social_network {

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

class ComposePostHandler : public ComposePostServiceIf {
 public:
  ComposePostHandler(
		NebulaClientPool<FakeRedisClient> *redis_pool,
		NebulaClientPool<PostStorageServiceClient> *post_storage_pool,
		NebulaClientPool<UserTimelineServiceClient> *user_timeline_pool,
		NebulaClientPool<FakeRabbitmqClient> *rabbitmq_pool);
	~ComposePostHandler() override = default;

  int64_t UploadText(int64_t req_id, const std::string& text) override;

  int64_t UploadMedia(int64_t req_id, const std::vector<Media>& media) override;

  int64_t UploadUniqueId(int64_t req_id, const int64_t post_id,
      const PostType::type post_type) override;

  int64_t UploadCreator(int64_t req_id, const Creator& creator) override;

  int64_t UploadUrls(int64_t req_id, const std::vector<Url> & urls) override;

  int64_t UploadUserMentions(const int64_t req_id,
      const std::vector<UserMention> & user_mentions) override;

	NebulaClientPool<FakeRedisClient> *_redis_pool;
	NebulaClientPool<PostStorageServiceClient> *_post_storage_pool;
	NebulaClientPool<UserTimelineServiceClient> *_user_timeline_pool;
	NebulaClientPool<FakeRabbitmqClient> *_rabbitmq_pool;
 
 private:

  void _ComposeAndUpload(int64_t req_id);

  void _UploadUserTimelineHelper(int64_t req_id, int64_t post_id,
      int64_t user_id, int64_t timestamp);

  void _UploadPostHelper(int64_t req_id, const Post &post);

  void _UploadHomeTimelineHelper(int64_t req_id, int64_t post_id,
      int64_t user_id, int64_t timestamp,
      const std::vector<int64_t> &user_mentions_id);

};

ComposePostHandler::ComposePostHandler(
		NebulaClientPool<FakeRedisClient> *redis_pool,
		NebulaClientPool<PostStorageServiceClient> *post_storage_pool,
		NebulaClientPool<UserTimelineServiceClient> *user_timeline_pool,
		NebulaClientPool<FakeRabbitmqClient> *rabbitmq_pool) {

	_redis_pool = redis_pool;
	_post_storage_pool = post_storage_pool;
	_user_timeline_pool = user_timeline_pool;
	_rabbitmq_pool = rabbitmq_pool;
}

int64_t ComposePostHandler::UploadCreator(
    int64_t req_id,
    const Creator &creator) {

	int64_t num_components;
	
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HS_CREATOR);
		redis_client->HSetCreator(req_id, "creator", creator);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HS_CREATOR);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetCreator(req_id, "creator", creator);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
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
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadText(
    int64_t req_id,
    const std::string &text) {

	int64_t num_components;
	
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HS_TEXT);
		redis_client->HSetText(req_id, "text", text);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HS_TEXT);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetText(req_id, "text", text);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
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
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadMedia(
    int64_t req_id,
    const std::vector<Media> &media) {

	int64_t num_components;

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HS_MEDIA);
		redis_client->HSetMedia(req_id, "media", media);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HS_MEDIA);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetMedia(req_id, "media", media);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
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
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadUniqueId(
    int64_t req_id,
    const int64_t post_id,
    const PostType::type post_type) {

	int64_t num_components;
	
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HS_POST_ID);
		redis_client->HSetPostId(req_id, "post_id", post_id);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HS_POST_ID);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetPostId(req_id, "post_id", post_id);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
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
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HS_POST_TYPE);
		redis_client->HSetPostType(req_id, "post_type", post_type);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HS_POST_TYPE);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetPostType(req_id, "post_type", post_type);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
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
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadUrls(
    int64_t req_id,
    const std::vector<Url> &urls) {

	int64_t num_components;

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HS_URLS);
		redis_client->HSetUrls(req_id, "urls", urls);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HS_URLS);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetUrls(req_id, "urls", urls);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
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
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadUserMentions(
    const int64_t req_id,
    const std::vector<UserMention> &user_mentions) {

	int64_t num_components;

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HS_USER_MENTIONS);
		redis_client->HSetUserMentions(req_id, "user_mentions", user_mentions);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HS_USER_MENTIONS);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetUserMentions(req_id, "user_mentions", user_mentions);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
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
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		#else
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	#ifdef __aarch64__
		NESTED_END();
	#endif

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

void ComposePostHandler::_ComposeAndUpload(
    int64_t req_id) {

	std::string text;
  Creator creator;
  std::vector<Media> media;
  int64_t post_id;
  std::vector<Url> urls;
  std::vector<UserMention> user_mentions;
  PostType::type post_type;


	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRedis
	try {
		#ifdef CEREBROS
		auto redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HG_TEXT);
		redis_client->HGetText(text, req_id, "text");

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HG_CREATOR);
		redis_client->HGetCreator(creator, req_id, "creator");

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HG_MEDIA);
		redis_client->HGetMedia(media, req_id, "media");

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HG_POST_ID);
		post_id = redis_client->HGetPostId(req_id, "post_id");

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HG_URLS);
		redis_client->HGetUrls(urls, req_id, "urls");

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HG_USER_MENTIONS);
		redis_client->HGetUserMentions(user_mentions, req_id, "user_mentions");

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client = _redis_pool->Get(FakeRedisIf::FuncType::HG_POST_TYPE);
		post_type = redis_client->HGetPostType(req_id, "post_type");
		
		#else

		auto redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HG_TEXT);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetText(text, req_id, "text");
		redis_client_wrapper->ResetBuffers(true, false);

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HG_CREATOR);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetCreator(creator, req_id, "creator");
		redis_client_wrapper->ResetBuffers(true, false);

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HG_MEDIA);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetMedia(media, req_id, "media");
		redis_client_wrapper->ResetBuffers(true, false);

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HG_POST_ID);
		redis_client = redis_client_wrapper->GetClient();
		post_id = redis_client->HGetPostId(req_id, "post_id");
		redis_client_wrapper->ResetBuffers(true, false);

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HG_URLS);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetUrls(urls, req_id, "urls");
		redis_client_wrapper->ResetBuffers(true, false);

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HG_USER_MENTIONS);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetUserMentions(user_mentions, req_id, "user_mentions");
		redis_client_wrapper->ResetBuffers(true, false);

	#ifdef __aarch64__
		NESTED_END();
	#endif

	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif

		redis_client_wrapper = _redis_pool->Get(FakeRedisIf::FuncType::HG_POST_TYPE);
		redis_client = redis_client_wrapper->GetClient();
		post_type = redis_client->HGetPostType(req_id, "post_type");
		redis_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
	
	#ifdef __aarch64__
		NESTED_END();
	#endif
  
  // Compose the post
  Post post;
  post.req_id = req_id;
  post.text = text;
  post.post_id = post_id;
  post.timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count();
  post.post_type = post_type;
  post.creator = creator;

  std::vector<int64_t> user_mentions_id;
  for (auto &mention : user_mentions) {
    user_mentions_id.emplace_back(mention.user_id);  
  }
  post.user_mentions = user_mentions;
  post.media = media;
  post.urls = urls;

  // Upload the post
  _UploadPostHelper(req_id, post);

  _UploadUserTimelineHelper(req_id, post.post_id, post.creator.user_id, post.timestamp);

  _UploadHomeTimelineHelper(req_id, post.post_id, post.creator.user_id, post.timestamp,
                            std::ref(user_mentions_id));
}

void ComposePostHandler::_UploadPostHelper(
    int64_t req_id,
    const Post &post) {
  
	#ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
  // Connect to PostStorageService
	try {
		#ifdef CEREBROS
		auto post_storage_client = _post_storage_pool->Get(PostStorageServiceIf::FuncType::STORE_POST);
		post_storage_client->StorePost(req_id, post);
		#else
		auto post_storage_client_wrapper = _post_storage_pool->Get(PostStorageServiceIf::FuncType::STORE_POST);
		auto post_storage_client = post_storage_client_wrapper->GetClient();
		post_storage_client->StorePost(req_id, post);
		post_storage_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to connect to post-storage-service:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
	#ifdef __aarch64__
		NESTED_END();
	#endif
}

void ComposePostHandler::_UploadUserTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp) {
  
  #ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to UserTimeLineService
	try {
		#ifdef CEREBROS
		auto user_timeline_client = _user_timeline_pool->Get(UserTimelineServiceIf::FuncType::WRITE_TIMELINE);
		user_timeline_client->WriteUserTimeline(req_id, post_id, user_id, timestamp);
		#else
		auto user_timeline_client_wrapper = _user_timeline_pool->Get(UserTimelineServiceIf::FuncType::WRITE_TIMELINE);
		auto user_timeline_client = user_timeline_client_wrapper->GetClient();
		user_timeline_client->WriteUserTimeline(req_id, post_id, user_id, timestamp);
		user_timeline_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to connect to user-timeline-service:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
	#ifdef __aarch64__
		NESTED_END();
	#endif
}

void ComposePostHandler::_UploadHomeTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp,
    const std::vector<int64_t> &user_mentions_id) {
  
  #ifdef __aarch64__
		NESTED_BEGIN();
		NESTED_DISPATCH_BEGIN();
	#endif
	// Connect to FakeRabbitMQ
	try {
		#ifdef CEREBROS
		auto rabbitmq_client = _rabbitmq_pool->Get(FakeRabbitmqIf::FuncType::UPLOAD_TIMELINE);
		rabbitmq_client->UploadHomeTimeline(req_id, post_id, user_id, timestamp, user_mentions_id);
		#else
		auto rabbitmq_client_wrapper = _rabbitmq_pool->Get(FakeRabbitmqIf::FuncType::UPLOAD_TIMELINE);
		auto rabbitmq_client = rabbitmq_client_wrapper->GetClient();
		rabbitmq_client->UploadHomeTimeline(req_id, post_id, user_id, timestamp, user_mentions_id);
		rabbitmq_client_wrapper->ResetBuffers(true, false);
		#endif
	} catch(const std::exception& e) {
		LOG(error) << "Failed to connect to home-timeline-rabbitmq:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
	#ifdef __aarch64__
		NESTED_END();
	#endif
}

} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_COMPOSEPOSTHANDLER_H_
