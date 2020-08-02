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

void ClientSendComposePost(MyThriftClient<MyComposePostServiceClient> *composePostClient){
  
  composePostClient->Connect();
  auto client = composePostClient->GetClient();

  int64_t req_id = 12345678; // TODO rand!

  client->send_UploadText(req_id, "This is a sample post!");
}

void ClientRecvComposePost(MyThriftClient<MyComposePostServiceClient> *composePostClient){
  
  composePostClient->Connect();
  auto client = composePostClient->GetClient();

  client->recv_UploadText();
}

void GenAndProcessComposePostReqs(MyThriftClient<MyComposePostServiceClient> *composePostClient,
                                  std::shared_ptr<MyComposePostHandler> handler,
                                  int tid, int max_tid) {

  auto srvIProt = composePostClient->GetClient()->getOutputProtocol();
  auto srvOProt = composePostClient->GetClient()->getInputProtocol();


  std::shared_ptr<MyComposePostServiceProcessor> processor =
      std::make_shared<MyComposePostServiceProcessor>(handler);

  uint64_t count = num_iterations;

  #ifdef FLEXUS
  if (tid == max_tid) {
    start = true;
    BREAKPOINT();
  }

  while(!start);
  #endif

  while (count--){

    #ifdef FLEXUS
      SKIP_BEGIN();
    #endif
    ClientSendComposePost(composePostClient);
    #ifdef FLEXUS
      SKIP_END();
    #endif

    // std::cout << "Processing Thread " << tid << " count=" << count+1  << std::endl;
    processor->process(srvIProt, srvOProt, nullptr);

    #ifdef FLEXUS
      SKIP_BEGIN();
    #endif
    ClientRecvComposePost(composePostClient);
    #ifdef FLEXUS
      SKIP_END();
    #endif
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

  for (int i = 0; i < num_threads; i++) {
    composePostClients[i] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);

    processThreads[i] = std::thread(GenAndProcessComposePostReqs,
                                      composePostClients[i], handler, i, num_threads - 1);

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