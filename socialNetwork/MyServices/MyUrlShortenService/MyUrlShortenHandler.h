#ifndef SOCIAL_NETWORK_MICROSERVICES_SRC_URLSHORTENSERVICE_MYURLSHORTENHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_SRC_URLSHORTENSERVICE_MYURLSHORTENHANDLER_H_

#include <random>
#include <chrono>
#include <future>

#include "../gen-cpp/MyUrlShortenService.h"
// #include "../ClientPool.h"
// #include "../ThriftClient.h"
// #include "../logger.h"
// #include "../tracing.h"

#define HOSTNAME "http://short-url/"

namespace social_network {

class MyUrlShortenHandler : virtual public MyUrlShortenServiceIf {
 public:
  
  MyUrlShortenHandler() {}
  // ~MyUrlShortenHandler() = default;

  void UploadUrls(std::vector<std::string> &, int64_t,
      const std::vector<std::string> &);

  void GetExtendedUrls(std::vector<std::string> &, int64_t,
                       const std::vector<std::string> &) ;

 private:
  static std::mt19937 _generator;
  static std::uniform_int_distribution<int> _distribution;
  static std::string _GenRandomStr(int length);
};

std::mt19937 MyUrlShortenHandler::_generator = std::mt19937(
    std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> MyUrlShortenHandler::_distribution =
    std::uniform_int_distribution<int>(0, 61);

// MyUrlShortenHandler::MyUrlShortenHandler() {
// }

std::string MyUrlShortenHandler::_GenRandomStr(int length) {
  const char char_map[] = "abcdefghijklmnopqrstuvwxyzABCDEF"
                    "GHIJKLMNOPQRSTUVWXYZ0123456789";
  std::string return_str;
  for (int i = 0; i < length; ++i) {
    return_str.append(1, char_map[_distribution(_generator)]);
  }
  return return_str;
}
void MyUrlShortenHandler::UploadUrls(
    std::vector<std::string> &_return,
    int64_t req_id,
    const std::vector<std::string> &urls) {

  std::vector<Url> target_urls;
  // std::future<void> mongo_future;

  if (!urls.empty()) {
    for (auto &url : urls) {
      Url new_target_url;
      new_target_url.expanded_url = url;
      new_target_url.shortened_url = HOSTNAME +
          MyUrlShortenHandler::_GenRandomStr(10);
      target_urls.emplace_back(new_target_url);
      _return.emplace_back(new_target_url.shortened_url);
    }
  }

  // std::future<void> compose_future = std::async(
  //     std::launch::async, [&]() {
  //       // Upload to compose post service
  //       auto compose_post_client_wrapper = _compose_client_pool->Pop();
  //       if (!compose_post_client_wrapper) {
  //         ServiceException se;
  //         se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
  //         se.message = "Failed to connect to compose-post-service";
  //         throw se;
  //       }
  //       auto compose_post_client = compose_post_client_wrapper->GetClient();
  //       try {
  //         // compose_post_client->UploadUrls(req_id, target_urls, writer_text_map);
  //       } catch (...) {
  //         _compose_client_pool->Push(compose_post_client_wrapper);
  //         LOG(error) << "Failed to upload urls to compose-post-service";
  //         throw;
  //       }
  //       _compose_client_pool->Push(compose_post_client_wrapper);
  //     });

  // try {
  //   compose_future.get();
  // } catch (...) {
  //   LOG(error) << "Failed to upload shortened urls from compose-post-service";
  //   throw;
  // }
}
void MyUrlShortenHandler::GetExtendedUrls(
    std::vector<std::string> &_return,
    int64_t req_id,
    const std::vector<std::string> &shortened_id) {

  // TODO: Implement GetExtendedUrls
}

}



#endif //SOCIAL_NETWORK_MICROSERVICES_SRC_URLSHORTENSERVICE_MYURLSHORTENHANDLER_H_
