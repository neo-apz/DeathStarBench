#ifndef SOCIAL_NETWORK_MICROSERVICES_MYCOMPOSEPOSTHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_MYCOMPOSEPOSTHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "../../gen-cpp/MyComposePostService.h"

#include "../../MyCommon/logger.h"

#include "../../MyCommon/RandomGenerator.h"

#define NUM_COMPONENTS 6

#define RAND_NUM_LIMIT 0xFFFFFFFFFFFFFF

#define NUM_ELEMENTS 10
namespace my_social_network {
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

class MyComposePostHandler : public MyComposePostServiceIf {
 public:
  MyComposePostHandler();
  ~MyComposePostHandler() override = default;

  int64_t UploadText(int64_t req_id, const std::string& text) override;

  int64_t UploadMedia(int64_t req_id, const std::vector<Media>& media) override;

  int64_t UploadUniqueId(int64_t req_id, const int64_t post_id,
      const PostType::type post_type) override;

  int64_t UploadCreator(int64_t req_id, const Creator& creator) override;

  int64_t UploadUrls(int64_t req_id, const std::vector<Url> & urls) override;

  int64_t UploadUserMentions(const int64_t req_id,
      const std::vector<UserMention> & user_mentions) override;

 private:

  std::string texts[NUM_ELEMENTS];
  Creator creators[NUM_ELEMENTS];
  std::vector<Media> media_vectors[NUM_ELEMENTS];
  int64_t post_ids[NUM_ELEMENTS];
  std::vector<Url> urls[NUM_ELEMENTS];
  std::vector<UserMention> user_mentions[NUM_ELEMENTS];
  PostType::type post_types[NUM_ELEMENTS];

  int64_t _ComposeAndUpload(int64_t req_id);

  int64_t _UploadUserTimelineHelper(int64_t req_id, int64_t post_id,
      int64_t user_id, int64_t timestamp);

  int64_t _UploadPostHelper(int64_t req_id, const Post &post);

  int64_t _UploadHomeTimelineHelper(int64_t req_id, int64_t post_id,
      int64_t user_id, int64_t timestamp,
      const std::vector<int64_t> &user_mentions_id);

};

MyComposePostHandler::MyComposePostHandler() {

  RandomGenerator randGen(1);

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    this->texts[i] = randGen.getAlphaNumericString(80);
    
    
    this->creators[i].user_id = randGen.getInt64(RAND_NUM_LIMIT);
    this->creators[i].username = randGen.getAlphaNumericString(12);

    uint32_t iters = randGen.getUInt32(1, 2);
    for(int i=0; i < iters; i++){
      Media *media = new Media();
      media->media_id = randGen.getInt64(RAND_NUM_LIMIT);
      media->media_type = randGen.getAlphaString(10);
      this->media_vectors[i].emplace_back(*media);
    }

    post_ids[i] = randGen.getInt64(RAND_NUM_LIMIT);

    iters = randGen.getUInt32(1, 2);
    for (int i = 0; i < iters; i++){
      Url *url = new Url();
      url->expanded_url = randGen.getAlphaNumericString(60);
      url->shortened_url = randGen.getAlphaNumericString(25);
      this->urls[i].emplace_back(*url);
    }

    iters = randGen.getUInt32(1, 2);
    for (int i = 0; i < iters; i++){
      UserMention *user_mention = new UserMention();
      user_mention->user_id = randGen.getInt64(RAND_NUM_LIMIT);
      user_mention->username = randGen.getAlphaNumericString(12);
      this->user_mentions[i].emplace_back(*user_mention);
    }

    this->post_types[i] = (PostType::type) randGen.getInt64(0, 3);
  }
}

int64_t MyComposePostHandler::UploadCreator(
    int64_t req_id,
    const Creator &creator) {

  std::string field = "creator";
  std::string incr_field = "num_components";
  int64_t num_components = (req_id + creator.user_id) % 6;

  if (num_components == 0) {
    return _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t MyComposePostHandler::UploadText(
    int64_t req_id,
    const std::string &text) {

  std::string field = "text";
  std::string incr_field = "num_components";
  int64_t num_components = req_id % 6;

  if (num_components == 0) {
    return _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t MyComposePostHandler::UploadMedia(
    int64_t req_id,
    const std::vector<Media> &media) {

  std::string field = "media";
  std::string incr_field = "num_components";
  int64_t num_components = (req_id + media.size()) % 6;

  if (num_components == 0) {
    return _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t MyComposePostHandler::UploadUniqueId(
    int64_t req_id,
    const int64_t post_id,
    const PostType::type post_type) {

  std::string field1 = "post_id";
  std::string field2 = "post_type";
  std::string incr_field = "num_components";
  int64_t num_components = (req_id + post_id) % 6;

  if (num_components == 0) {
    return _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t MyComposePostHandler::UploadUrls(
    int64_t req_id,
    const std::vector<Url> &urls) {

  std::string field = "urls";
  std::string incr_field = "num_components";
  int64_t num_components = (req_id + urls.size()) % 6;

  if (num_components == 0) {
    return _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t MyComposePostHandler::UploadUserMentions(
    const int64_t req_id,
    const std::vector<UserMention> &user_mentions) {

  std::string field = "user_mentions";
  std::string incr_field = "num_components";
  int64_t num_components = (req_id + user_mentions.size()) % 6;

  if (num_components == 0) {
    return _ComposeAndUpload(req_id);
  }

  return req_id;
}

int64_t MyComposePostHandler::_ComposeAndUpload(
    int64_t req_id) {

  std::string field1 = "text";
  std::string field2 = "creator";
  std::string field3 = "media";
  std::string field4 = "post_id";
  std::string field5 = "urls";
  std::string field6 = "user_mentions";
  std::string field7 = "post_type";

  int index = req_id % NUM_ELEMENTS;

  std::string text = this->texts[index];
  Creator creator = this->creators[index];
  std::vector<Media> media = this->media_vectors[index];
  int64_t post_id = this->post_ids[index];
  std::vector<Url> urls = this->urls[index];
  std::vector<UserMention> user_mentions = this->user_mentions[index];
  PostType::type post_type = this->post_types[index];

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
  int64_t ret = _UploadPostHelper(req_id, post);

  ret += _UploadUserTimelineHelper(req_id, post.post_id, post.creator.user_id, post.timestamp);

  ret += _UploadHomeTimelineHelper(req_id, post.post_id, post.creator.user_id, post.timestamp,
                            user_mentions_id);

  return ret;
}

int64_t MyComposePostHandler::_UploadPostHelper(
    int64_t req_id,
    const Post &post) {
  
  return req_id;
}

int64_t MyComposePostHandler::_UploadUserTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp) {
  
  return req_id + user_id - post_id + timestamp;
}

int64_t MyComposePostHandler::_UploadHomeTimelineHelper(
    int64_t req_id,
    int64_t post_id,
    int64_t user_id,
    int64_t timestamp,
    const std::vector<int64_t> &user_mentions_id) {
  
  return req_id + user_id + post_id - timestamp;
}

} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_MYCOMPOSEPOSTHANDLER_H_
