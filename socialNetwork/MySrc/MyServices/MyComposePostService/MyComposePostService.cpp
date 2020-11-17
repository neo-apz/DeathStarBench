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

#ifdef SW
double *throughputs;
double *latencies;
#endif

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
  int64_t req_id, MsgType::type msg_type, RandomGenerator* randGen){
  
  reqGenPhaseClient->Connect();
  auto client1 = reqGenPhaseClient->GetClient();
  auto client2 = processPhaseClient->GetClient();

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

void GenAndProcessComposePostReqs(MyThriftClient<MyComposePostServiceClient> **reqGenPhaseClients,
                                  MyThriftClient<MyComposePostServiceClient> **processPhaseClients,
                                  std::shared_ptr<MyComposePostHandler> handler,
                                  int64_t req_id,
                                  int tid, int max_tid) {

  std::shared_ptr<MyComposePostServiceProcessor> processor =
      std::make_shared<MyComposePostServiceProcessor>(handler);

  FakePostStorageServiceClient::isReqGenPhase = true;
  FakeRedisClient::isReqGenPhase = true;
  FakeRabbitmqClient::isReqGenPhase = true;
  FakeUserTimelineServiceClient::isReqGenPhase = true;

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

      ClientSendComposePost(reqGenPhaseClients[client_count], processPhaseClients[client_count], req_id, msg_type, &randGen);
      
      srvIProt = reqGenPhaseClients[client_count]->GetClient()->getOutputProtocol();
      srvOProt = reqGenPhaseClients[client_count]->GetClient()->getInputProtocol();
      processor->process(srvIProt, srvOProt, nullptr);

      ClientRecvComposePost(reqGenPhaseClients[client_count], msg_type);
      client_count++;
    }
      
    iter_count++;
    req_id++;
  }

  // std::shared_ptr<MyComposePostServiceProcessor> processPhaseprocessor =
  //     std::make_shared<MyComposePostServiceProcessor>(handler);

  pthread_barrier_wait(&barrier);
  if (tid == max_tid) {
    #ifdef FLEXUS
    BREAKPOINT();
    #endif
    start = true;
    LOG(warning) << "Process Phase Started!!";
  }

  while(!start);

  client_count = 0;
  FakePostStorageServiceClient::isReqGenPhase = false;
  FakeRedisClient::isReqGenPhase = false;
  FakeRabbitmqClient::isReqGenPhase = false;
  FakeUserTimelineServiceClient::isReqGenPhase = false;

  iterations = iterations * MsgType::SIZE;

  #ifdef SW
  Stopwatch<std::chrono::microseconds> sw;
  sw.start();
  #endif

  while (client_count < iterations){
    srvIProt = processPhaseClients[client_count]->GetClient()->getOutputProtocol();
    srvOProt = processPhaseClients[client_count]->GetClient()->getInputProtocol();

    // msg_type = (MsgType::type) (count % MsgType::SIZE);

    // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;
    // cout << "Process " << count << " finished!" << endl;
    
    processor->process(srvIProt, srvOProt, nullptr);

    // ClientRecvComposePost(composePostClient, &req_id, count);
    // if (msg_type == MsgType::USER_MENTIONS) {
    //   req_id++;
    // }

    client_count++;
    #ifdef __aarch64__
      PROCESS_END(client_count);
    #endif
  }

  if (tid == max_tid)
    LOG(warning) << "Process Phase finished!";

  #ifdef SW
  sw.stop();
  sw.post_process();
  // LOG(warning) << "[" << tid << "] AVG (us) = " <<  ((sw.mean() * 1.0) / num_iterations);
  throughputs[tid] = (num_iterations / (sw.mean() * 1.0));
  latencies[tid] = (sw.mean() * 1.0) / num_iterations;
  // LOG(warning) << "[" << tid << "] Million Reqs/s = " <<  throughputs[tid];
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

  pthread_barrier_init(&barrier, NULL, num_threads);

  MyThriftClient<MyComposePostServiceClient>** reqGenPhaseClients[num_threads];
  MyThriftClient<MyComposePostServiceClient>** processPhaseClients[num_threads];

  
  uint64_t buffer_size = num_iterations * BUFFER_SIZE;
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

  #ifdef SW
  throughputs = (double*) malloc(sizeof(double) * num_threads);
  latencies = (double*) malloc(sizeof(double) * num_threads);
  #endif

  buffer_size = BUFFER_SIZE;
  for (int i = 0; i < num_threads; i++) {
    #ifdef SW
    throughputs[i] = 0;
    latencies[i] = 0;
    #endif

    reqGenPhaseClients[i] = (MyThriftClient<MyComposePostServiceClient>**) malloc(sizeof(MyThriftClient<MyComposePostServiceClient>**) * num_iterations);
    processPhaseClients[i] = (MyThriftClient<MyComposePostServiceClient>**) malloc(sizeof(MyThriftClient<MyComposePostServiceClient>**) * num_iterations);

    for (int c = 0; c < num_iterations; c++) {
      reqGenPhaseClients[i][c] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);
      processPhaseClients[i][c] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);
    }

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

  #ifdef SW
  double total_throughput = 0;
  double avg_latency = 0;
  #endif

  for (int i = 0; i < num_threads; i++) {
    #ifdef SW
    total_throughput += throughputs[i];
    avg_latency += latencies[i];
    #endif
    for (int c = 0; c < num_iterations; c++) {
      delete reqGenPhaseClients[i][c];
      delete processPhaseClients[i][c];
    }
  }

  #ifdef SW
  std::cout << "Total throughput (Million RPS): " << total_throughput << std::endl;
  std::cout << "AVG latency (us): " << avg_latency / num_threads << std::endl;
  #endif

  return 0;
}