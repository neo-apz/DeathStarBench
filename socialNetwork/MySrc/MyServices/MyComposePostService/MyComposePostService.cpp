#include "../../MyCommon/utils.h"
#include "MyComposePostHandler.h"

#include "../../MyCommon/MyThriftClient.h"

#include "../../MyCommon/stopwatch.h"
#include "../../MyCommon/RandomGenerator.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif

#include "../../MyCommon/MyProcessorEventHandler.h"

using namespace my_social_network;

using namespace std;

uint64_t num_iterations;

cpu_set_t *cpuSet;

volatile bool start = false;
pthread_barrier_t barrier;

#define BUFFER_SIZE  220
#define REQ_ID_BEGIN 0xFFFFFFFFFFFF
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
  MyThriftClient<MyComposePostServiceClient> *clientPtr,
  int64_t req_id, MsgType::type msg_type, RandomGenerator* randGen){
  
  clientPtr->Connect();
  auto client = clientPtr->GetClient();

  switch (msg_type) {
    case MsgType::TEXT:
      {
        string str = randGen->getAlphaNumericString(80);
        client->send_UploadText(req_id, str);
      }
      break;
    case MsgType::MEDIA:
      {
        std::vector<Media> media_vector;
        uint32_t iters = randGen->getUInt32(1, 2);
        for(int i=0; i < iters; i++){
          Media media;
          media.media_id = randGen->getInt64(RAND_NUM_LIMIT);
          media.media_type = randGen->getAlphaString(10);
          media_vector.emplace_back(media);
        }
        client->send_UploadMedia(req_id, media_vector);
      }
      break;
    case MsgType::UNIQUE_ID:
      {
        int64_t id = randGen->getInt64(RAND_NUM_LIMIT);
        PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);
        client->send_UploadUniqueId(req_id, id, post_type);
      }
      break;
    case MsgType::CREATOR :
      {
        Creator creator;
        creator.user_id = randGen->getInt64(RAND_NUM_LIMIT);
        creator.username = randGen->getAlphaNumericString(12);
        client->send_UploadCreator(req_id, creator);
      }
      break;
    case MsgType::URLS :
      {
        std::vector<Url> urls;
        uint32_t iters = randGen->getUInt32(1, 2);
        for (int i = 0; i < iters; i++){
          Url url;
          url.expanded_url = randGen->getAlphaNumericString(60);
          url.shortened_url = randGen->getAlphaNumericString(25);
          urls.emplace_back(url);
        }
        client->send_UploadUrls(req_id, urls);
      }
      break;
    case MsgType::USER_MENTIONS :
      {
        std::vector<UserMention> user_mentions;
        uint32_t iters = randGen->getUInt32(1, 2);
        for (int i = 0; i < iters; i++){
          UserMention user_mention;
          user_mention.user_id = randGen->getInt64(RAND_NUM_LIMIT);
          user_mention.username = randGen->getAlphaNumericString(12);
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

void GenAndProcessComposePostReqs(MyThriftClient<MyComposePostServiceClient> **clientPtrs,
                                  std::shared_ptr<MyComposePostHandler> handler,
                                  int64_t req_id,
                                  int tid, int max_tid) {

  std::shared_ptr<MyComposePostServiceProcessor> processor =
      std::make_shared<MyComposePostServiceProcessor>(handler);

  RandomGenerator randGen(tid);

  uint64_t client_count = 0;
  uint64_t iter_count = 0;

  uint64_t iterations = num_iterations / MsgType::SIZE;
  
  uint32_t msg_types[MsgType::SIZE];
  apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> srvIProt, srvOProt;
  while (iter_count < iterations){
    randGen.getUInt32Set(msg_types, MsgType::SIZE);

    for (int i = 0; i < MsgType::SIZE; i++){
      MsgType::type msg_type = (MsgType::type) msg_types[i];

      ClientSendComposePost(clientPtrs[client_count], req_id, msg_type, &randGen);
      
      client_count++;
    }
      
    iter_count++;
    req_id++;
  }

  std::shared_ptr<MyProcessorEventHandler> eventHandler;

  #ifdef SW
    eventHandler = std::make_shared<MyProcessorEventHandler>(&(processor->disSW));
  #else
    eventHandler = std::make_shared<MyProcessorEventHandler>();
  #endif

  processor->setEventHandler(eventHandler);

  pthread_barrier_wait(&barrier);
  if (tid == max_tid) {
    #ifdef FLEXUS
    BREAKPOINT();
    #endif
    start = true;
    // LOG(warning) << "Process Phase Started!!";
  }

  while(!start);

  client_count = 0;

  iterations = iterations * MsgType::SIZE;

  while (client_count < iterations){
    srvIProt = clientPtrs[client_count]->GetClient()->getOutputProtocol();
    srvOProt = clientPtrs[client_count]->GetClient()->getInputProtocol();

    // msg_type = (MsgType::type) (count % MsgType::SIZE);

    // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;
    // cout << "Process " << count << " finished!" << endl;
    
    #ifdef __aarch64__
      PROCESS_BEGIN(count);
    #endif

    processor->process(srvIProt, srvOProt, nullptr);

    #ifdef __aarch64__
      PROCESS_END(client_count);
    #endif

    client_count++;
  }

  // if (tid == max_tid)
  //   LOG(warning) << "Process Phase finished!";

  #ifdef SW
    eventHandler->printResults();
    processor->headerSW.post_process();
    processor->disSW.post_process();

    double headerTime = (processor->headerSW.mean() * 1.0);
    cout << "AVG HeaderParsing Latency (us): " << headerTime / 1000 << endl;

    double disTime = (processor->disSW.mean() * 1.0);
    cout << "AVG Dispatch Latency (us): " << disTime / 1000 << endl;
  #endif
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

  MyThriftClient<MyComposePostServiceClient>** clients[num_threads];
  
  std::shared_ptr<MyComposePostHandler> handler = std::make_shared<MyComposePostHandler>();

  std::thread processThreads[num_threads];

  int64_t req_id_begin = REQ_ID_BEGIN;

  cpuSet = (cpu_set_t*) malloc(sizeof(cpu_set_t) * num_threads);

  buffer_size = BUFFER_SIZE;
  for (int i = 0; i < num_threads; i++) {
    clients[i] = (MyThriftClient<MyComposePostServiceClient>**) malloc(sizeof(MyThriftClient<MyComposePostServiceClient>**) * num_iterations);

    for (int c = 0; c < num_iterations; c++) {
      clients[i][c] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);
    }

    processThreads[i] = std::thread(GenAndProcessComposePostReqs,
                                      clients[i],
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
    for (int c = 0; c < num_iterations; c++) {
      delete clients[i][c];
    }
  }

  return 0;
}