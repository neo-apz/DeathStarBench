#ifndef SOCIAL_NETWORK_MICROSERVICES_MYCOMPOSEPOSTHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_MYCOMPOSEPOSTHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "../../gen-cpp/MyComposePostService.h"
#include "../../gen-cpp/FakePostStorageService.h"
#include "../../gen-cpp/FakeUserTimelineService.h"
#include "../../gen-cpp/FakeRedis.h"
#include "../../gen-cpp/FakeRabbitmq.h"

#include "../../MyCommon/ClientPoolMap.h"

#include "../../MyCommon/logger.h"

#define NUM_COMPONENTS 6
#define REDIS_EXPIRE_TIME 10

namespace my_social_network {
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

class MyComposePostHandler : public MyComposePostServiceIf {
 public:
  MyComposePostHandler(
      ClientPoolMap<MyThriftClient<FakeRedisClient>> *,
      ClientPoolMap<MyThriftClient<FakePostStorageServiceClient>> *,
      ClientPoolMap<MyThriftClient<FakeUserTimelineServiceClient>> *,
      ClientPoolMap<MyThriftClient<FakeRabbitmqClient>> *);
  ~MyComposePostHandler() override = default;

  void UploadText(int64_t req_id, const std::string& text) override;

  void UploadMedia(int64_t req_id, const std::vector<Media>& media) override;

  void UploadUniqueId(int64_t req_id, const int64_t post_id,
      const PostType::type post_type) override;

  void UploadCreator(int64_t req_id, const Creator& creator) override;

  void UploadUrls(int64_t req_id, const std::vector<Url> & urls) override;

  void UploadUserMentions(const int64_t req_id,
      const std::vector<UserMention> & user_mentions) override;

 private:
  ClientPoolMap<MyThriftClient<FakeRedisClient>> *_redis_client_pool;
  ClientPoolMap<MyThriftClient<FakePostStorageServiceClient>>
      *_post_storage_client_pool;
  ClientPoolMap<MyThriftClient<FakeUserTimelineServiceClient>>
      *_user_timeline_client_pool;
  ClientPoolMap<MyThriftClient<FakeRabbitmqClient>> *_rabbitmq_client_pool;

  void _ComposeAndUpload(int64_t req_id);

  void _UploadUserTimelineHelper(int64_t req_id, int64_t post_id,
      int64_t user_id, int64_t timestamp);

  void _UploadPostHelper(int64_t req_id, const Post &post);

  void _UploadHomeTimelineHelper(int64_t req_id, int64_t post_id,
      int64_t user_id, int64_t timestamp,
      const std::vector<int64_t> &user_mentions_id);

};

MyComposePostHandler::MyComposePostHandler(
    ClientPoolMap<MyThriftClient<FakeRedisClient>>  * redis_client_pool,
    ClientPoolMap<MyThriftClient<FakePostStorageServiceClient>> *post_storage_client_pool,
    ClientPoolMap<MyThriftClient<FakeUserTimelineServiceClient>> *user_timeline_client_pool,
    ClientPoolMap<MyThriftClient<FakeRabbitmqClient>> *rabbitmq_client_pool) {
  _redis_client_pool = redis_client_pool;
  _post_storage_client_pool = post_storage_client_pool;
  _user_timeline_client_pool = user_timeline_client_pool;
  _rabbitmq_client_pool = rabbitmq_client_pool;
}

void MyComposePostHandler::UploadCreator(
    int64_t req_id,
    const Creator &creator) {

  std::string field = "creator";
  std::string incr_field = "num_components";
  int64_t num_components;

  auto redis_client_wrapper = _redis_client_pool->Pop();
  if (!redis_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_REDIS_ERROR;
    se.message = "Cannot connect to Redis server";
    throw se;
  }
  auto redis_client = redis_client_wrapper->GetClient();

  try{
    redis_client->HSetCreator(req_id, field, creator);
    num_components = redis_client->HIncrBy(req_id, incr_field, 1);
  } catch (...) {
    _redis_client_pool->Push(redis_client_wrapper);
    LOG(error) << "Failed to send requests to redis-service";
    throw;
  }

  _redis_client_pool->Push(redis_client_wrapper);

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }
}

void MyComposePostHandler::UploadText(
    int64_t req_id,
    const std::string &text) {

  std::string field = "text";
  std::string incr_field = "num_components";
  int64_t num_components;

  auto redis_client_wrapper = _redis_client_pool->Pop();
  if (!redis_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_REDIS_ERROR;
    se.message = "Cannot connect to Redis server";
    throw se;
  }
  auto redis_client = redis_client_wrapper->GetClient();

  try{
    redis_client->HSetText(req_id, field, text);
    num_components = redis_client->HIncrBy(req_id, incr_field, 1);
  } catch (...) {
    _redis_client_pool->Push(redis_client_wrapper);
    LOG(error) << "Failed to send requests to redis-service";
    throw;
  }

  _redis_client_pool->Push(redis_client_wrapper);

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }
}

void MyComposePostHandler::UploadMedia(
    int64_t req_id,
    const std::vector<Media> &media) {

  std::string field = "media";
  std::string incr_field = "num_components";
  int64_t num_components;

  auto redis_client_wrapper = _redis_client_pool->Pop();
  if (!redis_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_REDIS_ERROR;
    se.message = "Cannot connect to Redis server";
    throw se;
  }
  auto redis_client = redis_client_wrapper->GetClient();

  try{
    redis_client->HSetMedia(req_id, field, media);
    num_components = redis_client->HIncrBy(req_id, incr_field, 1);
  } catch (...) {
    _redis_client_pool->Push(redis_client_wrapper);
    LOG(error) << "Failed to send requests to redis-service";
    throw;
  }

  _redis_client_pool->Push(redis_client_wrapper);

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }
}

void MyComposePostHandler::UploadUniqueId(
    int64_t req_id,
    const int64_t post_id,
    const PostType::type post_type) {

  std::string field1 = "post_id";
  std::string field2 = "post_type";
  std::string incr_field = "num_components";
  int64_t num_components;

  auto redis_client_wrapper = _redis_client_pool->Pop();
  if (!redis_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_REDIS_ERROR;
    se.message = "Cannot connect to Redis server";
    throw se;
  }
  auto redis_client = redis_client_wrapper->GetClient();

  try{
    redis_client->HSetPostId(req_id, field1, post_id);
    redis_client->HSetPostType(req_id, field2, post_type);
    num_components = redis_client->HIncrBy(req_id, incr_field, 1);
  } catch (...) {
    _redis_client_pool->Push(redis_client_wrapper);
    LOG(error) << "Failed to send requests to redis-service";
    throw;
  }

  _redis_client_pool->Push(redis_client_wrapper);

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }
}

void MyComposePostHandler::UploadUrls(
    int64_t req_id,
    const std::vector<Url> &urls) {

  std::string field = "urls";
  std::string incr_field = "num_components";
  int64_t num_components;

  auto redis_client_wrapper = _redis_client_pool->Pop();
  if (!redis_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_REDIS_ERROR;
    se.message = "Cannot connect to Redis server";
    throw se;
  }
  auto redis_client = redis_client_wrapper->GetClient();

  try{
    redis_client->HSetUrls(req_id, field, urls);
    num_components = redis_client->HIncrBy(req_id, incr_field, 1);
  } catch (...) {
    _redis_client_pool->Push(redis_client_wrapper);
    LOG(error) << "Failed to send requests to redis-service";
    throw;
  }

  _redis_client_pool->Push(redis_client_wrapper);

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }
}

void MyComposePostHandler::UploadUserMentions(
    const int64_t req_id,
    const std::vector<UserMention> &user_mentions) {

  std::string field = "user_mentions";
  std::string incr_field = "num_components";
  int64_t num_components;

  auto redis_client_wrapper = _redis_client_pool->Pop();
  if (!redis_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_REDIS_ERROR;
    se.message = "Cannot connect to Redis server";
    throw se;
  }
  auto redis_client = redis_client_wrapper->GetClient();

  try{
    redis_client->HSetUserMentions(req_id, field, user_mentions);
    num_components = redis_client->HIncrBy(req_id, incr_field, 1);
  } catch (...) {
    _redis_client_pool->Push(redis_client_wrapper);
    LOG(error) << "Failed to send requests to redis-service";
    throw;
  }

  _redis_client_pool->Push(redis_client_wrapper);

  if (num_components == NUM_COMPONENTS) {
    _ComposeAndUpload(req_id);
  }
}

void MyComposePostHandler::_ComposeAndUpload(
    int64_t req_id) {

  std::string field1 = "text";
  std::string field2 = "creator";
  std::string field3 = "media";
  std::string field4 = "post_id";
  std::string field5 = "urls";
  std::string field6 = "user_mentions";
  std::string field7 = "post_type";

  std::string text;
  Creator creator;
  std::vector<Media> media;
  int64_t post_id;
  std::vector<Url> urls;
  std::vector<UserMention> user_mentions;
  PostType::type post_type;

  auto redis_client_wrapper = _redis_client_pool->Pop();
  if (!redis_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_REDIS_ERROR;
    se.message = "Cannot connect to Redis server";
    throw se;
  }
  auto redis_client = redis_client_wrapper->GetClient();

  try{
    redis_client->HGetText(text, req_id, field1);
    redis_client->HGetCreator(creator, req_id, field2);
    redis_client->HGetMedia(media, req_id, field3);
    post_id = redis_client->HGetPostId(req_id, field4);
    redis_client->HGetUrls(urls, req_id, field5);
    redis_client->HGetUserMentions(user_mentions, req_id, field6);
    post_type = redis_client->HGetPostType(req_id, field7);
  } catch (...) {
    _redis_client_pool->Push(redis_client_wrapper);
    LOG(error) << "Failed to send requests to redis-service";
    throw;
  }

  _redis_client_pool->Push(redis_client_wrapper);

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
                            user_mentions_id);
}

void MyComposePostHandler::_UploadPostHelper(
    int64_t req_id,
    const Post &post) {
  try{
    auto post_storage_client_wrapper = _post_storage_client_pool->Pop();
    if (!post_storage_client_wrapper) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
      se.message = "Failed to connect to post-storage-service";
      throw se;
    }
    auto post_storage_client = post_storage_client_wrapper->GetClient();
    try {
      post_storage_client->StorePost(req_id, post);
    } catch (...) {
      _post_storage_client_pool->Push(post_storage_client_wrapper);
      LOG(error) << "Failed to store post to post-storage-service";
      throw;
    }
    _post_storage_client_pool->Push(post_storage_client_wrapper);
  } catch (...) {
    LOG(error) << "Failed to connect to post-storage-service";
    throw;
  }
}

void MyComposePostHandler::_UploadUserTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp) {
  try{
    auto user_timeline_client_wrapper = _user_timeline_client_pool->Pop();
    if (!user_timeline_client_wrapper) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
      se.message = "Failed to connect to user-timeline-service";
      throw se;
    }
    auto user_timeline_client = user_timeline_client_wrapper->GetClient();
    try {
      user_timeline_client->WriteUserTimeline(req_id, post_id, user_id,
                                              timestamp);
    } catch (...) {
      _user_timeline_client_pool->Push(user_timeline_client_wrapper);
      throw;
    }
    _user_timeline_client_pool->Push(user_timeline_client_wrapper);
  } catch (...) {
    LOG(error) << "Failed to write user-timeline to user-timeline-service";
    throw;
  }
}

void MyComposePostHandler::_UploadHomeTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp,
    const std::vector<int64_t> &user_mentions_id) {
  
  auto rabbitmq_client_wrapper = _rabbitmq_client_pool->Pop();
  if (!rabbitmq_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_RABBITMQ_CONN_ERROR;
    se.message = "Failed to connect to home-timeline-rabbitmq";
    throw se;
  }
  auto rabbitmq_client = rabbitmq_client_wrapper->GetClient();
  try {
    rabbitmq_client->UploadHomeTimeline(req_id, post_id, user_id, timestamp,
                                        user_mentions_id);
  } catch (...) {
    _rabbitmq_client_pool->Push(rabbitmq_client_wrapper);
    LOG(error) << "Failed to connect to home-timeline-rabbitmq";
    throw;
  }
  
  _rabbitmq_client_pool->Push(rabbitmq_client_wrapper);
}

} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_MYCOMPOSEPOSTHANDLER_H_
