#include "../../MyCommon/utils.h"
#include "SimpleUniqueIdHandler.h"

#include "../../MyCommon/MyThriftClient.h"
// #include "../../MyCommon/MyLock.h"

#include "../../MyCommon/stopwatch.h"
#include "../../MyCommon/RandomGenerator.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif


using namespace my_social_network;

using namespace std;

uint64_t num_iterations;
std::mutex thread_lock;
// MyLock thread_lock;
std::string machine_id;

cpu_set_t *cpuSet;
double *throughputs;
double *latencies;

volatile bool start = false;
// volatile std::atomic_int start2(0);
pthread_barrier_t barrier;


#define BUFFER_SIZE  50
#define WARM_UP_ITER  100

void ClientSendUniqueId(MyThriftClient<MyUniqueIdServiceClient> *clientPtr,
                        RandomGenerator *randGen){

  int64_t req_id = randGen->getInt64(0xFFFFFFFFFFFFFF);
  PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

  auto client = clientPtr->GetClient();
  client->send_UploadUniqueId(req_id, post_type);
}

void ClientRecvUniqueId(MyThriftClient<MyUniqueIdServiceClient> *uniqueIdClient){
  
  uniqueIdClient->Connect();
  auto client = uniqueIdClient->GetClient();

  client->recv_UploadUniqueId();
}

void GenAndProcessUniqueIdReqs(MyThriftClient<MyUniqueIdServiceClient> *clientPtr,
                               std::shared_ptr<SimpleUniqueIdHandler> handler,
                               int tid, int max_tid){

  // LOG(warning) << "User TID: " << tid << " TID: " << std::this_thread::get_id();
  RandomGenerator randGen(tid);

  uint64_t count = 1;

  while (count <= num_iterations){
    ClientSendUniqueId(clientPtr, &randGen);
    count++;
  }

  std::shared_ptr<MyUniqueIdServiceProcessor> processor =
      std::make_shared<MyUniqueIdServiceProcessor>(handler);

  auto srvIProt = clientPtr->GetClient()->getOutputProtocol();
  auto srvOProt = clientPtr->GetClient()->getInputProtocol();

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

  count = 1;

  // Stopwatch<std::chrono::microseconds> sw;
  // sw.start();

  while (count <= num_iterations){

    processor->process(srvIProt, srvOProt, nullptr);

    #ifdef __aarch64__
      PROCESS_END(count);
    #endif
    // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;

    // ClientRecvUniqueId(uniqueIdClient);

    count++;
  }

  // if (tid == max_tid)
  //   LOG(warning) << "Process Phase finished!";

  // sw.stop();
  // sw.post_process();
  // LOG(warning) << "[" << tid << "] AVG (us) = " <<  ((sw.mean() * 1.0) / num_iterations);
  // throughputs[tid] = (num_iterations / (sw.mean() * 1.0));
  // latencies[tid] = (sw.mean() * 1.0) / num_iterations;
  // LOG(warning) << "[" << tid << "] Million Reqs/s = " <<  throughputs[tid];
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

  uint64_t buffer_size = num_iterations * BUFFER_SIZE;
  // std::cout << "Buffer size: " << buffer_size << std::endl;

  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  pthread_barrier_init(&barrier, NULL, num_threads);

  MyThriftClient<MyUniqueIdServiceClient>* clients[num_threads];

  std::shared_ptr<SimpleUniqueIdHandler> handler = std::make_shared<SimpleUniqueIdHandler>(
                                              &thread_lock, machine_id);

  std::thread processThreads[num_threads];

  cpuSet = (cpu_set_t*) malloc(sizeof(cpu_set_t) * num_threads);
  // throughputs = (double*) malloc(sizeof(double) * num_threads);
  // latencies = (double*) malloc(sizeof(double) * num_threads);

  for (int i = 0; i < num_threads; i++) {
    // throughputs[i] = 0;
    // latencies[i] = 0;
    clients[i] = new MyThriftClient<MyUniqueIdServiceClient>(buffer_size);

    processThreads[i] = std::thread(GenAndProcessUniqueIdReqs,
                                      clients[i],
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

  // double total_throughput = 0;
  // double avg_latency = 0;

  for (int i = 0; i < num_threads; i++) {
    // total_throughput += throughputs[i];
    // avg_latency += latencies[i];
    delete clients[i];
  }

  // std::cout << "Total throughput (Million RPS): " << total_throughput << std::endl;
  // std::cout << "AVG latency (us): " << avg_latency / num_threads << std::endl;

  return 0;
}
