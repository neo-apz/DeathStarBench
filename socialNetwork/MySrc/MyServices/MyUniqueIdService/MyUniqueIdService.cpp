#include "../../MyCommon/utils.h"
#include "MyUniqueIdHandler.h"

#include "../../MyCommon/MyThriftClient.h"
#include "../../MyCommon/MyLock.h"

#include "../../MyCommon/stopwatch.h"
#include "../../MyCommon/RandomGenerator.h"

#include "../../MyCommon/PostPSendStage.h"
#include "../../MyCommon/PrePRecvStage.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif

using namespace my_social_network;

using namespace std;

uint64_t num_iterations;
// std::mutex thread_lock;
MyLock thread_lock;
std::string machine_id;

#include "../../MyCommon/core_schedule.h"

#ifdef SW
double *throughputs;
double *latencies;
#endif

volatile bool start = false;
// volatile std::atomic_int start2(0);
pthread_barrier_t barrier;


#define BUFFER_SIZE  50
#define WARM_UP_ITER  100

void Allocate(MyThriftClient<MyUniqueIdServiceClient> **reqGenPhaseClients,
             MyThriftClient<MyUniqueIdServiceClient> **processPhaseClients,
             uint64_t buffer_size){

  // *reqGenPhaseClients = (MyThriftClient<MyUniqueIdServiceClient>**) malloc(sizeof(MyThriftClient<MyUniqueIdServiceClient>**) * num_iterations);
  // *processPhaseClients = (MyThriftClient<MyUniqueIdServiceClient>**) malloc(sizeof(MyThriftClient<MyUniqueIdServiceClient>**) * num_iterations);

  // for (int c = 0; c < num_iterations; c++) {
  //     reqGenPhaseClients[c] = new MyThriftClient<MyUniqueIdServiceClient>(buffer_size);
  //     processPhaseClients[c] = new MyThriftClient<MyUniqueIdServiceClient>(buffer_size);
  // }
}

void CleanUp(MyThriftClient<MyUniqueIdServiceClient> **reqGenPhaseClients,
             MyThriftClient<MyUniqueIdServiceClient> **processPhaseClients,
             PostPSendStage* postpSendStageHandler,
             PrePRecvStage* prepRecvStageHandler){

  for (int c = 0; c < num_iterations; c++) {
      delete reqGenPhaseClients[c];
      delete processPhaseClients[c];
  }
  free(reqGenPhaseClients);
  free(processPhaseClients);
  delete postpSendStageHandler;
  delete prepRecvStageHandler;
}

void ClientSendUniqueId(MyThriftClient<MyUniqueIdServiceClient> *reqGenPhaseClient,
                        MyThriftClient<MyUniqueIdServiceClient> *processPhaseClient,
                        RandomGenerator *randGen){
  
  reqGenPhaseClient->Connect();

  int64_t req_id = randGen->getInt64(0xFFFFFFFFFFFFFF);
  PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);

  auto client = reqGenPhaseClient->GetClient();
  client->send_UploadUniqueId(req_id, post_type);

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // reqGenPhaseClient->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

  client = processPhaseClient->GetClient();
  client->send_UploadUniqueId(req_id, post_type);
}

void ClientRecvUniqueId(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  client->recv_UploadUniqueId();
}

void GenAndProcessUniqueIdReqs(MyThriftClient<MyUniqueIdServiceClient> **reqGenPhaseClients,
                               MyThriftClient<MyUniqueIdServiceClient> **processPhaseClients,
                               std::shared_ptr<MyUniqueIdHandler> handler,
                               int tid, int max_tid,
                               uint64_t buffer_size,
                               PostPSendStage* postpSendStageHandler,
                               PrePRecvStage* prepRecvStageHandler){

  // LOG(warning) << "User TID: " << tid << " TID: " << std::this_thread::get_id();

  Allocate(reqGenPhaseClients, reqGenPhaseClients, buffer_size);

#ifdef STAGED
  std::shared_ptr<MyUniqueIdServiceProcessor> processor =
      std::make_shared<MyUniqueIdServiceProcessor>(handler, postpSendStageHandler, prepRecvStageHandler);
  prepRecvStageHandler->setProcessor(processor);
  int completion;
#else
  std::shared_ptr<MyUniqueIdServiceProcessor> processor =
    std::make_shared<MyUniqueIdServiceProcessor>(handler, postpSendStageHandler, prepRecvStageHandler);
#endif

  FakeComposePostServiceClient::isReqGenPhase = true;
  RandomGenerator randGen(tid);

  uint64_t count = 0;

  apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> srvIProt, srvOProt;

  while (count < num_iterations){
    
    ClientSendUniqueId(reqGenPhaseClients[count], processPhaseClients[count], &randGen);

    srvIProt = reqGenPhaseClients[count]->GetClient()->getOutputProtocol();
    srvOProt = reqGenPhaseClients[count]->GetClient()->getInputProtocol();

    #ifdef STAGED
    prepRecvStageHandler->EnqueuePrePReq(srvIProt, srvOProt);

    while (processor->_postpSendStageHandler->PeekPostP() == nullptr);
    processor->_postpSendStageHandler->PostPCompletion(completion);
    #else
    processor->process(srvIProt, srvOProt, nullptr);
    #endif
    
    ClientRecvUniqueId(reqGenPhaseClients[count]);
    count++;
    // std::cout << "ReqGen Thread " << tid << " count=" << count  << std::endl;
  }

  // std::shared_ptr<MyUniqueIdServiceProcessor> processPhaseprocessor =
  //     std::make_shared<MyUniqueIdServiceProcessor>(handler);

  // LOG(warning) << "ReqGen Phase finished!";

  pthread_barrier_wait(&barrier);
  // start2++;

  // while (start2 < max_tid+1);

  if (tid == max_tid) {
    #ifdef FLEXUS
    BREAKPOINT();
    #endif
    start = true;
    // LOG(warning) << "Process Phase Started!!";
  }

  while(!start);
  // LOG(warning) << "Process Phase Started!!";

  count = 0;
  FakeComposePostServiceClient::isReqGenPhase = false;

  #ifdef SW
  Stopwatch<std::chrono::nanoseconds> prepSW_;
  Stopwatch<std::chrono::microseconds> sw;
  sw.start();
  #endif

  while (count < num_iterations){

    // #ifdef SW
    // prepSW_.start();
    // #endif
    
    srvIProt = processPhaseClients[count]->GetClient()->getOutputProtocol();
    srvOProt = processPhaseClients[count]->GetClient()->getInputProtocol();

    #ifdef STAGED
    prepRecvStageHandler->EnqueuePrePReq(srvIProt, srvOProt);
    #else
    processor->process(srvIProt, srvOProt, nullptr);
    #ifdef __aarch64__
    PROCESS_END(count);
    #endif
    #endif

    count++;
        
    // #ifdef SW
    // prepSW_.stop();
    // #endif
  }

  #ifdef STAGED
  count = 0;
  while (count < num_iterations){
    if (processor->_postpSendStageHandler->PeekPostP() != nullptr){ // request is done
      processor->_postpSendStageHandler->PostPCompletion(completion);
      count++;
      // std::cout << "After CQ Check Thread " << tid << " count=" << count  << std::endl;
      #ifdef __aarch64__
      PROCESS_END(count);
      #endif
    }
  }
  #endif

  #ifdef SW
  sw.stop();
  sw.post_process();
  // LOG(warning) << "[" << tid << "] AVG (us) = " <<  ((sw.mean() * 1.0) / num_iterations);
  throughputs[tid] = (num_iterations / (sw.mean() * 1.0));
  latencies[tid] = (sw.mean() * 1.0) / num_iterations;
  // LOG(warning) << "[" << tid << "] Million Reqs/s = " <<  throughputs[tid];
  // prepSW_.post_process();
  // std::cout << "PreP: " << prepSW_.mean() << std::endl;
  #endif

  // if (tid == max_tid)
  //   LOG(warning) << "Process Phase finished!";

  count = 0;
  while (count < num_iterations){
    ClientRecvUniqueId(processPhaseClients[count]);
    count++;
  }

  CleanUp(reqGenPhaseClients, processPhaseClients, postpSendStageHandler, prepRecvStageHandler);
}

int main(int argc, char *argv[]) {
  init_logger();

  uint64_t num_threads;

  if (argc != 3) {
    cout << "Invalid input! Usage: ./MyUniqueIdService <num_threads> <iterations> \n" << endl;
    exit(-1);
  } else {
    num_threads = atoi(argv[1]);
    num_iterations = atoi(argv[2]);
  }

  cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  pthread_barrier_init(&barrier, NULL, num_threads);

  MyThriftClient<MyUniqueIdServiceClient>** reqGenPhaseClients[num_threads];
  MyThriftClient<MyUniqueIdServiceClient>** processPhaseClients[num_threads];

  PrePRecvStage* prepRecvStageHandlers[num_threads];
  PostPSendStage* postpSendStageHandlers[num_threads]; 

  for (int i = 0; i < num_threads; i++) {
    prepRecvStageHandlers[i] = new PrePRecvStage();
    postpSendStageHandlers[i] = new PostPSendStage(prepRecvStageHandlers[i]);
  }

  uint64_t buffer_size = BUFFER_SIZE * num_iterations;
  // std::cout << "Buffer size: " << buffer_size << std::endl;
  ClientPoolMap<MyThriftClient<FakeComposePostServiceClient>> fakeComposeClientPool (
    "compose-post", buffer_size, num_threads,
    postpSendStageHandlers, prepRecvStageHandlers);
  
  // MyClientPool<MyThriftClient<FakeComposePostServiceClient>> fakeComposeClientPool (
  //   "compose-post", buffer_size, 2, 2, 1000);


  std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
                                              &thread_lock, machine_id,
                                              &fakeComposeClientPool);

  std::thread processThreads[num_threads];

  // cpuSet = (cpu_set_t*) malloc(sizeof(cpu_set_t) * num_threads);
  
  #ifdef SW
  throughputs = (double*) malloc(sizeof(double) * num_threads);
  latencies = (double*) malloc(sizeof(double) * num_threads);
  #endif

  buffer_size = BUFFER_SIZE;
  int coreId = -1;
  for (int i = 0; i < num_threads; i++) {
    #ifdef SW
    throughputs[i] = 0;
    latencies[i] = 0;
    #endif

    reqGenPhaseClients[i] = (MyThriftClient<MyUniqueIdServiceClient>**) malloc(sizeof(MyThriftClient<MyUniqueIdServiceClient>**) * num_iterations);
    processPhaseClients[i] = (MyThriftClient<MyUniqueIdServiceClient>**) malloc(sizeof(MyThriftClient<MyUniqueIdServiceClient>**) * num_iterations);

    for (int c = 0; c < num_iterations; c++) {
      reqGenPhaseClients[i][c] = new MyThriftClient<MyUniqueIdServiceClient>(buffer_size);
      processPhaseClients[i][c] = new MyThriftClient<MyUniqueIdServiceClient>(buffer_size);
    }

    processThreads[i] = std::thread(GenAndProcessUniqueIdReqs,
                                      reqGenPhaseClients[i],
                                      processPhaseClients[i],
                                      handler,
                                      i,
                                      num_threads - 1,
                                      buffer_size,
                                      postpSendStageHandlers[i],
                                      prepRecvStageHandlers[i]);

    coreId = PinToCore(&processThreads[i]);
    // std::cout << "Processor thread pinned to core " << coreId << "." << std::endl;
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
  }

  #ifdef SW
  std::cout << "Total throughput (Million RPS): " << total_throughput << std::endl;
  std::cout << "AVG latency (us): " << avg_latency / num_threads << std::endl;
  #endif

  return 0;
}
