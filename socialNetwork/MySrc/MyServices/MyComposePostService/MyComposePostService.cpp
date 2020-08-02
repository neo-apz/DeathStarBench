#include "../../MyCommon/utils.h"
#include "MyComposePostHandler.h"

#include "../../MyCommon/MyClientPool.h"
#include "../../MyCommon/MyThriftClient.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif

using namespace my_social_network;

using namespace std;

uint64_t num_iterations;

#ifdef __aarch64__
  cpu_set_t cpuSet[16];
#endif

#ifdef FLEXUS
  volatile bool start = false;
#endif

#define BUFFER_SIZE  50
#define REQ_ID_BEGIN 1234567898765432

struct MsgType {
  enum type {
    USER_MENTIONS = 0,
    TEXT = 1,
    MEDIA = 2,
    UNIQUE_ID = 3,
    CREATOR = 4,
    URLS = 5,
    SIZE = 6
  };
};

void ClientSendComposePost(
  MyThriftClient<MyComposePostServiceClient> *composePostClient,
  int64_t req_id, uint64_t iter_count){
  
  composePostClient->Connect();
  auto client = composePostClient->GetClient();

  MsgType::type msg_type = (MsgType::type) (iter_count % MsgType::SIZE);

  switch (msg_type)
  {
  case MsgType::TEXT :
    client->send_UploadText(req_id, "This is a just sample post, nothing more, nothing less!");
    break;
  case MsgType::MEDIA:
    {
      std::vector<Media> media_vector;
      
      for(int i=0; i < 2; i++){
        Media media;
        media.media_id = 12345;
        media.media_type = "Video";
        media_vector.emplace_back(media);
      }
      client->send_UploadMedia(req_id, media_vector);
    }
    break;
  case MsgType::UNIQUE_ID :
    client->send_UploadUniqueId(req_id, 123456789087, PostType::POST);
    break;
  case MsgType::CREATOR :
    {
      Creator creator;
      creator.user_id = 9876543210;
      creator.username = "sample_username";
      client->send_UploadCreator(req_id, creator);
    }
    break;
  case MsgType::URLS :
    {
      std::vector<Url> urls;
      for (int i = 0; i < 2; i++){
        Url url;
        url.expanded_url = "http://www.expandedURL.com/thisisthefullurlinitsexpandedformat";
        url.shortened_url = "http://www.short.en/XCBNHJKL";
        urls.emplace_back(url);
      }
      client->send_UploadUrls(req_id, urls);
    }
    break;
  case MsgType::USER_MENTIONS :
    {
      std::vector<UserMention> user_mentions;
      for (int i = 0; i < 2; i++){
        UserMention user_mention;
        user_mention.user_id = 12345689876543210;
        user_mention.username = "my_username";
        user_mentions.emplace_back(user_mention);
      }
      client->send_UploadUserMentions(req_id, user_mentions);
    }
    break;    
  default:
    cout << "This is an error, wrong message type!" << endl;
    exit(1);
  }

}

void ClientRecvComposePost(
  MyThriftClient<MyComposePostServiceClient> *composePostClient,
  int64_t *req_id, uint64_t iter_count){
  
  composePostClient->Connect();
  auto client = composePostClient->GetClient();

  MsgType::type msg_type = (MsgType::type) (iter_count % MsgType::SIZE);

  switch (msg_type)
  {
  case MsgType::TEXT :
    client->recv_UploadText();
    break;
  case MsgType::MEDIA :
    client->recv_UploadMedia();
    break;
  case MsgType::UNIQUE_ID :
    client->recv_UploadUniqueId();
    break;
  case MsgType::CREATOR :
    client->recv_UploadCreator();
    break;
  case MsgType::URLS :
    client->recv_UploadUrls();
    break;
  case MsgType::USER_MENTIONS :
    client->recv_UploadUserMentions();
    *req_id = (*req_id) + 1;
    break;    
  default:
    cout << "This is an error, wrong message type!" << endl;
    exit(1);
  }

}

void GenAndProcessComposePostReqs(MyThriftClient<MyComposePostServiceClient> *composePostClient,
                                  std::shared_ptr<MyComposePostHandler> handler,
                                  int64_t req_id,
                                  int tid, int max_tid) {

  auto srvIProt = composePostClient->GetClient()->getOutputProtocol();
  auto srvOProt = composePostClient->GetClient()->getInputProtocol();


  std::shared_ptr<MyComposePostServiceProcessor> processor =
      std::make_shared<MyComposePostServiceProcessor>(handler);

  uint64_t count = 1;

  #ifdef FLEXUS
  if (tid == max_tid) {
    start = true;
    BREAKPOINT();
  }

  while(!start);
  #endif

  while (count <= num_iterations){

    #ifdef FLEXUS
      SKIP_BEGIN();
    #endif
    ClientSendComposePost(composePostClient, req_id, count);
    #ifdef FLEXUS
      SKIP_END();
    #endif

    // std::cout << "Processing Thread " << tid << " count=" << count+1  << std::endl;
    processor->process(srvIProt, srvOProt, nullptr);

    #ifdef FLEXUS
      SKIP_BEGIN();
    #endif
    ClientRecvComposePost(composePostClient, &req_id, count);
    #ifdef FLEXUS
      SKIP_END();
    #endif

    count++;
  }

}

int main(int argc, char *argv[]) {
  init_logger();

  uint64_t num_threads;

  if (argc != 3) {
    cout << "Invalid input! Usage: ./MyComposePostService <num_threads> <iterations> \n" << endl;
    exit(-1);
  } else {
    num_threads = atoi(argv[1]);
    num_iterations = atoi(argv[2]);
  }

  #ifdef __aarch64__
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
  #endif

  uint64_t buffer_size = num_iterations * BUFFER_SIZE;

  MyThriftClient<MyComposePostServiceClient>* composePostClients[num_threads];

  MyClientPool<MyThriftClient<FakeRedisClient>> fakeRedisClientPool (
    "redis", buffer_size, 16, 16, 1000);

  MyClientPool<MyThriftClient<FakePostStorageServiceClient>> fakePostStorageClientPool (
    "post-storage", buffer_size, 16, 16, 1000);

  MyClientPool<MyThriftClient<FakeUserTimelineServiceClient>> fakeUserTimelineClientPool (
    "user-timeline", buffer_size, 16, 16, 1000);    

  MyClientPool<MyThriftClient<FakeRabbitmqClient>> fakeRabbitmqClientPool (
    "rabbitmq", buffer_size, 16, 16, 1000);


  std::shared_ptr<MyComposePostHandler> handler = std::make_shared<MyComposePostHandler>(
                                                            &fakeRedisClientPool,
                                                            &fakePostStorageClientPool,
                                                            &fakeUserTimelineClientPool,
                                                            &fakeRabbitmqClientPool);


  std::thread processThreads[num_threads];

  int64_t req_id_begin = REQ_ID_BEGIN;

  for (int i = 0; i < num_threads; i++) {
    composePostClients[i] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);

    processThreads[i] = std::thread(GenAndProcessComposePostReqs,
                                      composePostClients[i], handler, req_id_begin,
                                      i, num_threads - 1);

    req_id_begin += num_iterations + 2;

    #ifdef __aarch64__
      CPU_ZERO(&cpuSet[i]);
      CPU_SET(i+1, &cpuSet[i]);
      pthread_setaffinity_np(processThreads[i].native_handle(), sizeof(cpu_set_t), &cpuSet[i]);
    #endif                                  
  }

  for (int i = 0; i < num_threads; i++) {
    processThreads[i].join();
  }

  for (int i = 0; i < num_threads; i++) {
    delete composePostClients[i];
  }

  return 0;
}