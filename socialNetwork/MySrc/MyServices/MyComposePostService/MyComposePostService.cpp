#include "../../MyCommon/utils.h"
#include "MyComposePostHandler.h"

#include "../../MyCommon/MyThriftClient.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif

using namespace my_social_network;

using namespace std;

uint64_t num_iterations;

cpu_set_t *cpuSet;

volatile bool start = false;
pthread_barrier_t barrier;

#define BUFFER_SIZE  220
#define REQ_ID_BEGIN 1234567898765432
#define WARM_UP_ITER  100

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
  MyThriftClient<MyComposePostServiceClient> *reqGenPhaseClient,
  MyThriftClient<MyComposePostServiceClient> *processPhaseClient,
  int64_t req_id, MsgType::type msg_type){
  
  reqGenPhaseClient->Connect();
  auto client1 = reqGenPhaseClient->GetClient();
  auto client2 = processPhaseClient->GetClient();

  switch (msg_type)
  {
  case MsgType::TEXT :
    client1->send_UploadText(req_id, "This is a just sample post, nothing more, nothing less!");
    client2->send_UploadText(req_id, "This is a just sample post, nothing more, nothing less!");
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
      client1->send_UploadMedia(req_id, media_vector);
      client2->send_UploadMedia(req_id, media_vector);
    }
    break;
  case MsgType::UNIQUE_ID :
    client1->send_UploadUniqueId(req_id, 123456789087, PostType::POST);
    client2->send_UploadUniqueId(req_id, 123456789087, PostType::POST);
    break;
  case MsgType::CREATOR :
    {
      Creator creator;
      creator.user_id = 9876543210;
      creator.username = "sample_username";
      client1->send_UploadCreator(req_id, creator);
      client2->send_UploadCreator(req_id, creator);
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
      client1->send_UploadUrls(req_id, urls);
      client2->send_UploadUrls(req_id, urls);
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
      client1->send_UploadUserMentions(req_id, user_mentions);
      client2->send_UploadUserMentions(req_id, user_mentions);
    }
    break;    
  default:
    cout << "This is an error, wrong message type!" << endl;
    exit(1);
  }

}

void ClientRecvComposePost(
  MyThriftClient<MyComposePostServiceClient> *composePostClient, MsgType::type msg_type){
  
  composePostClient->Connect();
  auto client = composePostClient->GetClient();

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
    break;    
  default:
    cout << "This is an error, wrong message type!" << endl;
    exit(1);
  }

}

void GenAndProcessComposePostReqs(MyThriftClient<MyComposePostServiceClient> *reqGenPhaseClient,
                                  MyThriftClient<MyComposePostServiceClient> *processPhaseClient,
                                  std::shared_ptr<MyComposePostHandler> handler,
                                  int64_t req_id,
                                  int tid, int max_tid) {

  auto srvIProt = reqGenPhaseClient->GetClient()->getOutputProtocol();
  auto srvOProt = reqGenPhaseClient->GetClient()->getInputProtocol();


  std::shared_ptr<MyComposePostServiceProcessor> reqGenprocessor =
      std::make_shared<MyComposePostServiceProcessor>(handler);

  FakePostStorageServiceClient::isReqGenPhase = true;
  FakeRedisClient::isReqGenPhase = true;
  FakeRabbitmqClient::isReqGenPhase = true;
  FakeUserTimelineServiceClient::isReqGenPhase = true;

  uint64_t count = 1;

  MsgType::type msg_type;
  
  while (count <= num_iterations){

    msg_type = (MsgType::type) (count % MsgType::SIZE);

    ClientSendComposePost(reqGenPhaseClient, processPhaseClient, req_id, msg_type);

    reqGenprocessor->process(srvIProt, srvOProt, nullptr);
    
    ClientRecvComposePost(reqGenPhaseClient, msg_type);
    count++;

    if (msg_type == MsgType::USER_MENTIONS) {
      req_id++;
    }

  }

  std::shared_ptr<MyComposePostServiceProcessor> processPhaseprocessor =
      std::make_shared<MyComposePostServiceProcessor>(handler);

  srvIProt = processPhaseClient->GetClient()->getOutputProtocol();
  srvOProt = processPhaseClient->GetClient()->getInputProtocol();

  pthread_barrier_wait(&barrier);

  if (tid == max_tid) {
    #ifdef FLEXUS
    BREAKPOINT();
    #endif
    start = true;
  }

  while(!start);

  count = 1;
  FakePostStorageServiceClient::isReqGenPhase = false;
  FakeRedisClient::isReqGenPhase = false;
  FakeRabbitmqClient::isReqGenPhase = false;
  FakeUserTimelineServiceClient::isReqGenPhase = false;

  while (count <= num_iterations){

    // msg_type = (MsgType::type) (count % MsgType::SIZE);

    // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;
    // cout << "Process " << count << " finished!" << endl;
    processPhaseprocessor->process(srvIProt, srvOProt, nullptr);

    #ifdef __aarch64__
      PROCESS_END(count);
    #endif

    // ClientRecvComposePost(composePostClient, &req_id, count);
    // if (msg_type == MsgType::USER_MENTIONS) {
    //   req_id++;
    // }

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

  cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  uint64_t buffer_size = num_iterations * BUFFER_SIZE;

  pthread_barrier_init(&barrier, NULL, num_threads);

  MyThriftClient<MyComposePostServiceClient>* reqGenPhaseClients[num_threads];
  MyThriftClient<MyComposePostServiceClient>* processPhaseClients[num_threads];

  ClientPoolMap<MyThriftClient<FakeRedisClient>> fakeRedisClientPool (
    "redis", buffer_size, num_threads);

  ClientPoolMap<MyThriftClient<FakePostStorageServiceClient>> fakePostStorageClientPool (
    "post-storage", buffer_size * 2, num_threads);

  ClientPoolMap<MyThriftClient<FakeUserTimelineServiceClient>> fakeUserTimelineClientPool (
    "user-timeline", buffer_size, num_threads);    

  ClientPoolMap<MyThriftClient<FakeRabbitmqClient>> fakeRabbitmqClientPool (
    "rabbitmq", buffer_size, num_threads);


  std::shared_ptr<MyComposePostHandler> handler = std::make_shared<MyComposePostHandler>(
                                                            &fakeRedisClientPool,
                                                            &fakePostStorageClientPool,
                                                            &fakeUserTimelineClientPool,
                                                            &fakeRabbitmqClientPool);


  std::thread processThreads[num_threads];

  int64_t req_id_begin = REQ_ID_BEGIN;

  cpuSet = (cpu_set_t*) malloc(sizeof(cpu_set_t) * num_threads);

  for (int i = 0; i < num_threads; i++) {
    reqGenPhaseClients[i] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);
    processPhaseClients[i] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);

    processThreads[i] = std::thread(GenAndProcessComposePostReqs,
                                      reqGenPhaseClients[i],
                                      processPhaseClients[i],
                                      handler,
                                      req_id_begin,
                                      i, num_threads - 1);

    req_id_begin += num_iterations + 2;

    CPU_ZERO(&cpuSet[i]);
    CPU_SET(i+1, &cpuSet[i]);
    pthread_setaffinity_np(processThreads[i].native_handle(), sizeof(cpu_set_t), &cpuSet[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    processThreads[i].join();
  }

  for (int i = 0; i < num_threads; i++) {
    delete reqGenPhaseClients[i];
    delete processPhaseClients[i];
  }

  return 0;
}