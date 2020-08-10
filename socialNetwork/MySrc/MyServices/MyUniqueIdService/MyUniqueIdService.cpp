#include "../../MyCommon/utils.h"
#include "MyUniqueIdHandler.h"

#include "../../MyCommon/MyThriftClient.h"
#include "../../MyCommon/MyLock.h"

#include "../../MyCommon/stopwatch.h"
#include "../../MyCommon/RandomGenerator.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif


using namespace my_social_network;

using namespace std;

uint64_t num_iterations;
// std::mutex thread_lock;
MyLock thread_lock;
std::string machine_id;

cpu_set_t *cpuSet;

#ifdef SW
double *throughputs;
double *latencies;
#endif

volatile bool start = false;
// volatile std::atomic_int start2(0);
pthread_barrier_t barrier;


#define BUFFER_SIZE  50
#define WARM_UP_ITER  100

void ClientSendUniqueId(MyThriftClient<MyUniqueIdServiceClient> *reqGenPhaseClient,
                        MyThriftClient<MyUniqueIdServiceClient> *processPhaseClient,
                        RandomGenerator *randGen){
  
  reqGenPhaseClient->Connect();

  int64_t req_id = randGen->getInt64(0xFFFFFFFFFFFFFF);
  PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);

  auto client = reqGenPhaseClient->GetClient();
  client->send_UploadUniqueId(req_id, post_type);

  uint8_t* cltIBufPtr, *cltOBufPtr;
  uint32_t ISz, OSz;
  
  reqGenPhaseClient->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

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
                               int tid, int max_tid){

  // LOG(warning) << "User TID: " << tid << " TID: " << std::this_thread::get_id();

#ifdef STAGED
  std::shared_ptr<MyUniqueIdServiceProcessor> processor =
      std::make_shared<MyUniqueIdServiceProcessor>(handler, tid+1);
#else
  std::shared_ptr<MyUniqueIdServiceProcessor> processor =
    std::make_shared<MyUniqueIdServiceProcessor>(handler);
#endif

  FakeComposePostServiceClient::isReqGenPhase = true;
  RandomGenerator randGen(tid);

  uint64_t count = 0;

  apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> srvIProt, srvOProt;

  while (count < num_iterations){
    
    ClientSendUniqueId(reqGenPhaseClients[count], processPhaseClients[count], &randGen);

    srvIProt = reqGenPhaseClients[count]->GetClient()->getOutputProtocol();
    srvOProt = reqGenPhaseClients[count]->GetClient()->getInputProtocol();

    processor->process(srvIProt, srvOProt, nullptr);
    
    ClientRecvUniqueId(reqGenPhaseClients[count]);
    // std::cout << "ReqGen Thread " << tid << " count=" << count  << std::endl;
    count++;
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
  Stopwatch<std::chrono::microseconds> sw;
  sw.start();
  #endif

  while (count < num_iterations){

    srvIProt = processPhaseClients[count]->GetClient()->getOutputProtocol();
    srvOProt = processPhaseClients[count]->GetClient()->getInputProtocol();

    processor->process(srvIProt, srvOProt, nullptr);

    // ClientRecvUniqueId(uniqueIdClient);
    
    count++;
    // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;
    #ifdef __aarch64__
      PROCESS_END(count);
    #endif
  }

  #ifdef SW
  sw.stop();
  sw.post_process();
  // LOG(warning) << "[" << tid << "] AVG (us) = " <<  ((sw.mean() * 1.0) / num_iterations);
  throughputs[tid] = (num_iterations / (sw.mean() * 1.0));
  latencies[tid] = (sw.mean() * 1.0) / num_iterations;
  // LOG(warning) << "[" << tid << "] Million Reqs/s = " <<  throughputs[tid];
  #endif

  // if (tid == max_tid)
  //   LOG(warning) << "Process Phase finished!";

  count = 0;
  while (count < num_iterations){
    ClientRecvUniqueId(processPhaseClients[count]);
    count++;
  }
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

  uint64_t buffer_size = BUFFER_SIZE * num_iterations;
  // std::cout << "Buffer size: " << buffer_size << std::endl;
  ClientPoolMap<MyThriftClient<FakeComposePostServiceClient>> fakeComposeClientPool (
    "compose-post", buffer_size, num_threads);
  
  // MyClientPool<MyThriftClient<FakeComposePostServiceClient>> fakeComposeClientPool (
  //   "compose-post", buffer_size, 2, 2, 1000);


  std::shared_ptr<MyUniqueIdHandler> handler = std::make_shared<MyUniqueIdHandler>(
                                              &thread_lock, machine_id,
                                              &fakeComposeClientPool);

  std::thread processThreads[num_threads];

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
                                      num_threads - 1);

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
