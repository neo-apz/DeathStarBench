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

#include "../../MyCommon/logger.h"

#include "../../MyCommon/RandomGenerator.h"

#include <NebulaClientPool.h>

#define NUM_COMPONENTS 6

#define NUM_ELEMENTS 10
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

  // uint64_t counter = 0;
  // std::mutex _counter_lock;

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

  std::string texts[NUM_ELEMENTS];
  Creator creators[NUM_ELEMENTS];
  std::vector<Media> media_vectors[NUM_ELEMENTS];
  int64_t post_ids[NUM_ELEMENTS];
  std::vector<Url> urls[NUM_ELEMENTS];
  std::vector<UserMention> user_mentions[NUM_ELEMENTS];
  PostType::type post_types[NUM_ELEMENTS];

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

  // RandomGenerator randGen(1);

  // for (int i = 0; i < NUM_ELEMENTS; i++) {
  //   this->texts[i] = randGen.getAlphaNumericString(80);
    
    
  //   this->creators[i].user_id = randGen.getInt64(RAND_NUM_LIMIT);
  //   this->creators[i].username = randGen.getAlphaNumericString(12);

  //   uint32_t iters = randGen.getUInt32(1, 2);
  //   for(int i=0; i < iters; i++){
  //     Media *media = new Media();
  //     media->media_id = randGen.getInt64(RAND_NUM_LIMIT);
  //     media->media_type = randGen.getAlphaString(10);
  //     this->media_vectors[i].emplace_back(*media);
  //   }

  //   post_ids[i] = randGen.getInt64(RAND_NUM_LIMIT);

  //   iters = randGen.getUInt32(1, 2);
  //   for (int i = 0; i < iters; i++){
  //     Url *url = new Url();
  //     url->expanded_url = randGen.getAlphaNumericString(60);
  //     url->shortened_url = randGen.getAlphaNumericString(25);
  //     this->urls[i].emplace_back(*url);
  //   }

  //   iters = randGen.getUInt32(1, 2);
  //   for (int i = 0; i < iters; i++){
  //     UserMention *user_mention = new UserMention();
  //     user_mention->user_id = randGen.getInt64(RAND_NUM_LIMIT);
  //     user_mention->username = randGen.getAlphaNumericString(12);
  //     this->user_mentions[i].emplace_back(*user_mention);
  //   }

  //   this->post_types[i] = (PostType::type) randGen.getInt64(0, 3);
  // }
}

int64_t ComposePostHandler::UploadCreator(
    int64_t req_id,
    const Creator &creator) {

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HS_CREATOR);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetCreator(req_id, "creator", creator);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	int64_t num_components;

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadText(
    int64_t req_id,
    const std::string &text) {

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HS_TEXT);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetText(req_id, "text", text);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	int64_t num_components;

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadMedia(
    int64_t req_id,
    const std::vector<Media> &media) {

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HS_MEDIA);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetMedia(req_id, "media", media);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	int64_t num_components;

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadUniqueId(
    int64_t req_id,
    const int64_t post_id,
    const PostType::type post_type) {

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HS_POST_ID);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetPostId(req_id, "post_id", post_id);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HS_POST_TYPE);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetPostType(req_id, "post_type", post_type);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	int64_t num_components;

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadUrls(
    int64_t req_id,
    const std::vector<Url> &urls) {

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HS_URLS);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetUrls(req_id, "urls", urls);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	int64_t num_components;

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t ComposePostHandler::UploadUserMentions(
    const int64_t req_id,
    const std::vector<UserMention> &user_mentions) {

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HS_USER_MENTIONS);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HSetUserMentions(req_id, "user_mentions", user_mentions);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Cannot connect to Redis server:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

	int64_t num_components;

	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::H_INC);
		auto redis_client = redis_client_wrapper->GetClient();
		num_components = redis_client->HIncrBy(req_id, "num_components", 1);
		redis_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}

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


	// Connect to FakeRedis
	try {
		auto redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HG_TEXT);
		auto redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetText(text, req_id, "text");
		redis_client_wrapper->ResetBuffers(true, false);

		redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HG_CREATOR);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetCreator(creator, req_id, "creator");
		redis_client_wrapper->ResetBuffers(true, false);

		redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HG_MEDIA);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetMedia(media, req_id, "media");
		redis_client_wrapper->ResetBuffers(true, false);

		redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HG_POST_ID);
		redis_client = redis_client_wrapper->GetClient();
		post_id = redis_client->HGetPostId(req_id, "post_id");
		redis_client_wrapper->ResetBuffers(true, false);

		redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HG_URLS);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetUrls(urls, req_id, "urls");
		redis_client_wrapper->ResetBuffers(true, false);

		redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HG_USER_MENTIONS);
		redis_client = redis_client_wrapper->GetClient();
		redis_client->HGetUserMentions(user_mentions, req_id, "user_mentions");
		redis_client_wrapper->ResetBuffers(true, false);

		redis_client_wrapper = _redis_pool->Get(FakeRedisClient::FuncType::HG_POST_TYPE);
		redis_client = redis_client_wrapper->GetClient();
		post_type = redis_client->HGetPostType(req_id, "post_type");
		redis_client_wrapper->ResetBuffers(true, false);

	} catch(const std::exception& e) {
		LOG(error) << "Failed to retrieve message from Redis:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
  
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
  
  // Connect to PostStorageService
	try {
		auto post_storage_client_wrapper = _post_storage_pool->Get(PostStorageServiceClient::FuncType::STORE_POST);
		auto post_storage_client = post_storage_client_wrapper->GetClient();
		post_storage_client->StorePost(req_id, post);
		post_storage_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to connect to post-storage-service:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
}

void ComposePostHandler::_UploadUserTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp) {
  
  // Connect to UserTimeLineService
	try {
		auto user_timeline_client_wrapper = _user_timeline_pool->Get(UserTimelineServiceClient::FuncType::WRITE_TIMELINE);
		auto user_timeline_client = user_timeline_client_wrapper->GetClient();
		user_timeline_client->WriteUserTimeline(req_id, post_id, user_id, timestamp);
		user_timeline_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to connect to user-timeline-service:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
}

void ComposePostHandler::_UploadHomeTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp,
    const std::vector<int64_t> &user_mentions_id) {
  
  // Connect to FakeRabbitMQ
	try {
		auto rabbitmq_client_wrapper = _rabbitmq_pool->Get(FakeRabbitmqClient::FuncType::UPLOAD_TIMELINE);
		auto rabbitmq_client = rabbitmq_client_wrapper->GetClient();
		rabbitmq_client->UploadHomeTimeline(req_id, post_id, user_id, timestamp, user_mentions_id);
		rabbitmq_client_wrapper->ResetBuffers(true, false);
	} catch(const std::exception& e) {
		LOG(error) << "Failed to connect to home-timeline-rabbitmq:\n"
							 << e.what() << '\n' ;
		exit(EXIT_FAILURE);
	}
}

} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_COMPOSEPOSTHANDLER_H_
