#include "../../MyCommon/utils.h"
#include "MyComposePostHandler.h"

#include "../../MyCommon/MyThriftClient.h"

#include "../../MyCommon/stopwatch.h"
#include "../../MyCommon/RandomGenerator.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif

using namespace my_social_network;

using namespace std;

uint64_t num_iterations;

cpu_set_t *cpuSet;
double *throughputs;
double *latencies;

volatile bool start = false;
pthread_barrier_t barrier;

#define BUFFER_SIZE  220
#define REQ_ID_BEGIN 0xFFFFFFFFFFFF
#define RAND_NUM_LIMIT 0xFFFFFFFFFFFFFF
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
  int64_t req_id, uint32_t* msg_types, RandomGenerator* randGen){
  
  reqGenPhaseClient->Connect();
  auto client1 = reqGenPhaseClient->GetClient();
  auto client2 = processPhaseClient->GetClient();

  MsgType::type msg_type;
  for (int i = 0; i < MsgType::SIZE; i++) {
    msg_type = (MsgType::type) msg_types[i];

    switch (msg_type) {
      case MsgType::TEXT:
        {
          string str = randGen->getAlphaNumericString(120);
          client1->send_UploadText(req_id, str);
          client2->send_UploadText(req_id, str);
        }
        break;
      case MsgType::MEDIA:
        {
          std::vector<Media> media_vector;
          uint32_t iters = randGen->getUInt32(1, 4);
          for(int i=0; i < iters; i++){
            Media media;
            media.media_id = randGen->getInt64(RAND_NUM_LIMIT);
            media.media_type = randGen->getAlphaString(10);
            media_vector.emplace_back(media);
          }
          client1->send_UploadMedia(req_id, media_vector);
          client2->send_UploadMedia(req_id, media_vector);
        }
        break;
      case MsgType::UNIQUE_ID:
        {
          int64_t id = randGen->getInt64(RAND_NUM_LIMIT);
          PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);
          client1->send_UploadUniqueId(req_id, id, post_type);
          client2->send_UploadUniqueId(req_id, id, post_type);
        }
        break;
      case MsgType::CREATOR :
        {
          Creator creator;
          creator.user_id = randGen->getInt64(RAND_NUM_LIMIT);
          creator.username = randGen->getAlphaNumericString(12);
          client1->send_UploadCreator(req_id, creator);
          client2->send_UploadCreator(req_id, creator);
        }
        break;
      case MsgType::URLS :
        {
          std::vector<Url> urls;
          uint32_t iters = randGen->getUInt32(1, 4);
          for (int i = 0; i < iters; i++){
            Url url;
            url.expanded_url = randGen->getAlphaNumericString(60);
            url.shortened_url = randGen->getAlphaNumericString(25);
            urls.emplace_back(url);
          }
          client1->send_UploadUrls(req_id, urls);
          client2->send_UploadUrls(req_id, urls);
        }
        break;
      case MsgType::USER_MENTIONS :
        {
          std::vector<UserMention> user_mentions;
          uint32_t iters = randGen->getUInt32(1, 4);
          for (int i = 0; i < iters; i++){
            UserMention user_mention;
            user_mention.user_id = randGen->getInt64(RAND_NUM_LIMIT);
            user_mention.username = randGen->getAlphaNumericString(12);
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
}

void ClientRecvComposePost(
  MyThriftClient<MyComposePostServiceClient> *composePostClient, uint32_t* msg_types){
  
  composePostClient->Connect();
  auto client = composePostClient->GetClient();

  MsgType::type msg_type;

  for (int i; i < MsgType::SIZE; i++) {
    msg_type = (MsgType::type) msg_types[i];
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

  RandomGenerator randGen(tid);

  uint64_t count = 1;

  uint64_t iterations = num_iterations / MsgType::SIZE;
  
  uint32_t msg_types[MsgType::SIZE];
  while (count <= iterations){
    randGen.getUInt32Set(msg_types, MsgType::SIZE);

    ClientSendComposePost(reqGenPhaseClient, processPhaseClient, req_id, msg_types, &randGen);

    for (int i = 0; i < MsgType::SIZE; i++)
      reqGenprocessor->process(srvIProt, srvOProt, nullptr);
    
    ClientRecvComposePost(reqGenPhaseClient, msg_types);
    count++;
    req_id++;
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

  Stopwatch<std::chrono::microseconds> sw;
  sw.start();

  iterations = iterations * MsgType::SIZE;
  while (count <= iterations){

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

  sw.stop();
  sw.post_process();
  // LOG(warning) << "[" << tid << "] AVG (us) = " <<  ((sw.mean() * 1.0) / num_iterations);
  throughputs[tid] = (num_iterations / (sw.mean() * 1.0));
  latencies[tid] = (sw.mean() * 1.0) / num_iterations;
  // LOG(warning) << "[" << tid << "] Million Reqs/s = " <<  throughputs[tid];

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
  throughputs = (double*) malloc(sizeof(double) * num_threads);
  latencies = (double*) malloc(sizeof(double) * num_threads);

  for (int i = 0; i < num_threads; i++) {
    throughputs[i] = 0;
    latencies[i] = 0;
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

  double total_throughput = 0;
  double avg_latency = 0;

  for (int i = 0; i < num_threads; i++) {
    total_throughput += throughputs[i];
    avg_latency += latencies[i];
    delete reqGenPhaseClients[i];
    delete processPhaseClients[i];
  }

  std::cout << "Total throughput (Million RPS): " << total_throughput << std::endl;
  std::cout << "AVG latency (us): " << avg_latency / num_threads << std::endl;

  return 0;
}