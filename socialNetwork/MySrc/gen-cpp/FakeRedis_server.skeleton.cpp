// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "FakeRedis.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::my_social_network;

class FakeRedisHandler : virtual public FakeRedisIf {
 public:
  FakeRedisHandler() {
    // Your initialization goes here
  }

  void HSetCreator(const int64_t req_id, const std::string& field, const Creator& creator) {
    // Your implementation goes here
    printf("HSetCreator\n");
  }

  void HSetText(const int64_t req_id, const std::string& field, const std::string& text) {
    // Your implementation goes here
    printf("HSetText\n");
  }

  void HSetMedia(const int64_t req_id, const std::string& field, const std::vector<Media> & media) {
    // Your implementation goes here
    printf("HSetMedia\n");
  }

  void HSetPostId(const int64_t req_id, const std::string& field, const int64_t post_id) {
    // Your implementation goes here
    printf("HSetPostId\n");
  }

  void HSetPostType(const int64_t req_id, const std::string& field, const PostType::type post_type) {
    // Your implementation goes here
    printf("HSetPostType\n");
  }

  void HSetUrls(const int64_t req_id, const std::string& field, const std::vector<Url> & urls) {
    // Your implementation goes here
    printf("HSetUrls\n");
  }

  void HSetUserMentions(const int64_t req_id, const std::string& field, const std::vector<UserMention> & user_mentions) {
    // Your implementation goes here
    printf("HSetUserMentions\n");
  }

  void HGetCreator(Creator& _return, const int64_t req_id, const std::string& field) {
    // Your implementation goes here
    printf("HGetCreator\n");
  }

  void HGetText(std::string& _return, const int64_t req_id, const std::string& field) {
    // Your implementation goes here
    printf("HGetText\n");
  }

  void HGetMedia(std::vector<Media> & _return, const int64_t req_id, const std::string& field) {
    // Your implementation goes here
    printf("HGetMedia\n");
  }

  int64_t HGetPostId(const int64_t req_id, const std::string& field) {
    // Your implementation goes here
    printf("HGetPostId\n");
  }

  PostType::type HGetPostType(const int64_t req_id, const std::string& field) {
    // Your implementation goes here
    printf("HGetPostType\n");
  }

  void HGetUrls(std::vector<Url> & _return, const int64_t req_id, const std::string& field) {
    // Your implementation goes here
    printf("HGetUrls\n");
  }

  void HGetUserMentions(std::vector<UserMention> & _return, const int64_t req_id, const std::string& field) {
    // Your implementation goes here
    printf("HGetUserMentions\n");
  }

  int64_t HIncrBy(const int64_t key, const std::string& field, const int64_t value) {
    // Your implementation goes here
    printf("HIncrBy\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  ::apache::thrift::stdcxx::shared_ptr<FakeRedisHandler> handler(new FakeRedisHandler());
  ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new FakeRedisProcessor(handler));
  ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
