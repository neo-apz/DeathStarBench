#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>


#include "../../gen-cpp/ComposePostService.h"


using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TCompactProtocolFactory;
using namespace social_network;


namespace social_network {

class ComposePostHandler : public ComposePostServiceIf {
 public:
  ComposePostHandler();
  ~ComposePostHandler() override = default;

  void UploadText(int64_t req_id, const std::string& text) override;

  void UploadMedia(int64_t req_id, const std::vector<Media>& media) override;

  void UploadUniqueId(int64_t req_id, const int64_t post_id,
      const PostType::type post_type) override;

  void UploadCreator(int64_t req_id, const Creator& creator) override;

  void UploadUrls(int64_t req_id, const std::vector<Url> & urls) override;

  void UploadUserMentions(const int64_t req_id,
      const std::vector<UserMention> & user_mentions) override;

};

ComposePostHandler::ComposePostHandler() {}

void ComposePostHandler::UploadCreator( int64_t req_id, const Creator &creator) {}

void ComposePostHandler::UploadText(int64_t req_id, const std::string &text) { }

void ComposePostHandler::UploadMedia(int64_t req_id, const std::vector<Media> &media) {}

void ComposePostHandler::UploadUniqueId(int64_t req_id, const int64_t post_id, const PostType::type post_type) {}

void ComposePostHandler::UploadUrls(int64_t req_id, const std::vector<Url> &urls) {}

void ComposePostHandler::UploadUserMentions(const int64_t req_id, const std::vector<UserMention> &user_mentions) {}


void sigintHandler(int sig) {
  exit(EXIT_SUCCESS);
}

}// namespace social_network

#define FIVE_MINS 1000*300

int main(int argc, char *argv[]) {
  signal(SIGINT, sigintHandler);

  int port = 5000;

  std::shared_ptr<TServerSocket> socket = std::shared_ptr<TServerSocket>(new TServerSocket("0.0.0.0", port));
  socket->setKeepAlive(true);
  socket->setAcceptTimeout(FIVE_MINS);
  socket->setRecvTimeout(FIVE_MINS);
  socket->setSendTimeout(FIVE_MINS);
  
  TThreadedServer server(
      std::make_shared<ComposePostServiceProcessor>(
          std::make_shared<ComposePostHandler>()),
      socket,
      std::make_shared<TFramedTransportFactory>(),
      std::make_shared<TCompactProtocolFactory>()
  );
  
  std::cout << "Starting the compose-post-service server ..." << std::endl;
  server.serve();

  return 0;
}