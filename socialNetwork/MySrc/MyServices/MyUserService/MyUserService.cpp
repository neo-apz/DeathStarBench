#include "../../MyCommon/utils.h"
#include "MyUserHandler.h"

#include "../../MyCommon/MyThriftClient.h"

#include "../../MyCommon/stopwatch.h"
#include "../../MyCommon/RandomGenerator.h"

#ifdef __aarch64__
  #include "../../MyCommon/MagicBreakPoint.h"
#endif

#include "../../MyCommon/MyProcessorEventHandler.h"

using namespace my_social_network;

using namespace std;

std::mutex thread_lock;
std::string secret = "secret";

uint64_t num_iterations;

cpu_set_t *cpuSet;

volatile bool start = false;
pthread_barrier_t barrier;

#define BUFFER_SIZE  220
#define REQ_ID_BEGIN 0xFFFFFFFFFFFF
#define WARM_UP_ITER  100

struct MsgType {
  enum type {
    REG_USER = 0,
    REG_USER_ID = 1,
    UP_CRT_ID = 2,
    UP_CRT_UNAME = 3,
    LOGIN = 4,
    GET_UID = 5,

    SIZE = 6
  };
};

int chances[] = {0,
                 1,
                 2, 2, 2, 2, 2, 2, 2, 2,
                 3, 3, 3, 3, 3, 3, 3, 3,
                 4, 4, 4, 4,
                 5, 5, 5, 5, 5};
int chances_size = 27;

void ClientSendReq(
  MyThriftClient<MyUserServiceClient> *clientPtr,
  int64_t req_id, RandomGenerator* randGen){
  
  clientPtr->Connect();
  auto client = clientPtr->GetClient();

  MsgType::type msg_type = (MsgType::type) chances[randGen->getUInt32(chances_size)];

  switch (msg_type) {
    case MsgType::REG_USER:
      {
        string first_name = randGen->getAlphaString(8);
        string last_name = randGen->getAlphaString(12);
        string username = randGen->getAlphaNumericString(12);
        string password = randGen->getAlphaNumericString(12);
        client->send_RegisterUser(req_id, first_name, last_name, username, password);
      }
      break;
    case MsgType::REG_USER_ID:
      {
        int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
        string first_name = randGen->getAlphaString(8);
        string last_name = randGen->getAlphaString(12);
        string username = randGen->getAlphaNumericString(12);
        string password = randGen->getAlphaNumericString(12);
        client->send_RegisterUserWithId(req_id, first_name, last_name, username, password, user_id);
      }
      break;
    case MsgType::UP_CRT_ID:
      {
        int64_t user_id = randGen->getInt64(RAND_NUM_LIMIT);
        string username = randGen->getAlphaNumericString(12);
        client->send_UploadCreatorWithUserId(req_id, user_id, username);
      }
      break;
    case MsgType::UP_CRT_UNAME:
      {
        string username = randGen->getAlphaNumericString(12);
        client->send_UploadCreatorWithUsername(req_id, username);
      }
      break;
    case MsgType::LOGIN:
      {
        string username = randGen->getAlphaNumericString(12);
        string password = randGen->getAlphaNumericString(12);
        client->send_Login(req_id, username, password);
      }
      break;
    case MsgType::GET_UID:
      {
        string username = randGen->getAlphaNumericString(12);
        client->send_GetUserId(req_id, username);
      }
      break;
    default:
      cout << "This is an error, wrong message type!" << endl;
      exit(1);
  }     
}

void ClientRecvReq(MyThriftClient<MyUserServiceClient> *clientPtr, MsgType::type msg_type){
  
  clientPtr->Connect();
  auto client = clientPtr->GetClient();

  // switch (msg_type)
  // {
  //   case MsgType::GET_FOLLOWERS :
  //     client->recv_GetFollowees();
  //     break;
  //   case MsgType::GET_FOLLOWEES :
  //     client->recv_UploadMedia();
  //     break;
  //   case MsgType::FOLLOW :
  //     client->recv_UploadUniqueId();
  //     break;
  //   case MsgType::UNFOLLOW :
  //     client->recv_UploadCreator();
  //     break;
  //   case MsgType::FOLLOW_UNAME :
  //     client->recv_UploadUrls();
  //     break;
  //   case MsgType::UNFOLLOW_UNAME :
  //     client->recv_UploadUserMentions();
  //     break;    
  //   case MsgType::IN_USER :
  //     client->recv_UploadUserMentions();
  //     break;    
  //   default:
  //     cout << "This is an error, wrong message type!" << endl;
  //     exit(1);
  // } 

}

void GenAndProcessReqs(MyThriftClient<MyUserServiceClient> *clientPtr,
                                  std::shared_ptr<MyUserHandler> handler,
                                  int64_t req_id,
                                  int tid, int max_tid) {

  std::shared_ptr<MyUserServiceProcessor> processor =
      std::make_shared<MyUserServiceProcessor>(handler);

  RandomGenerator randGen(tid);

  uint64_t iter_count = 0;
  
  // apache::thrift::stdcxx::shared_ptr<::apache::thrift::protocol::TProtocol> srvIProt, srvOProt;

  while (iter_count < num_iterations){
    ClientSendReq(clientPtr, req_id, &randGen);
      
    iter_count++;
    req_id++;
  }

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

  std::shared_ptr<MyProcessorEventHandler> eventHandler;

  #ifdef SW
    eventHandler = std::make_shared<MyProcessorEventHandler>(&(processor->disSW));
  #else
    eventHandler = std::make_shared<MyProcessorEventHandler>();
  #endif

  processor->setEventHandler(eventHandler);

  auto srvIProt = clientPtr->GetClient()->getOutputProtocol();
  auto srvOProt = clientPtr->GetClient()->getInputProtocol();

  pthread_barrier_wait(&barrier);
  if (tid == max_tid) {
    #ifdef FLEXUS
    BREAKPOINT();
    #endif
    start = true;
    // LOG(warning) << "Process Phase Started!!";
  }

  while(!start);

  iter_count = 0;

  while (iter_count < num_iterations){
    // srvIProt = clientPtrs[client_count]->GetClient()->getOutputProtocol();
    // srvOProt = clientPtrs[client_count]->GetClient()->getInputProtocol();

    // msg_type = (MsgType::type) (count % MsgType::SIZE);

    // std::cout << "Processing Thread " << tid << " count=" << count  << std::endl;
    // cout << "Process " << count << " finished!" << endl;
    
    #ifdef __aarch64__
      PROCESS_BEGIN(iter_count+1);
    #endif

    processor->process(srvIProt, srvOProt, nullptr);

    #ifdef __aarch64__
      PROCESS_END(iter_count+1);
    #endif

    iter_count++;
  }

  // if (tid == max_tid)
  //   LOG(warning) << "Process Phase finished!";

  // uint8_t* cltIBufPtr, *cltOBufPtr;
  // uint32_t ISz, OSz;
  
  // clientPtr->GetBuffer(&cltIBufPtr, &ISz, &cltOBufPtr, &OSz);

  // std::cout << "ISz: " <<  ISz << " OSz: " << OSz << std::endl;

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
    cout << "Invalid input! Usage: ./MyUserService <num_threads> <iterations> \n" << endl;
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

  std::string machine_id;
  if (GetMachineId(&machine_id) != 0) {
    exit(EXIT_FAILURE);
  }

  // MyThriftClient<MyComposePostServiceClient>** clients[num_threads];
  MyThriftClient<MyUserServiceClient>* clients[num_threads];
  
  std::shared_ptr<MyUserHandler> handler = std::make_shared<MyUserHandler>(
                                            &thread_lock,
                                            machine_id,
                                            secret);

  std::thread processThreads[num_threads];

  int64_t req_id_begin = REQ_ID_BEGIN;

  cpuSet = (cpu_set_t*) malloc(sizeof(cpu_set_t) * num_threads);

  // buffer_size = BUFFER_SIZE;
  for (int i = 0; i < num_threads; i++) {
    // clients[i] = (MyThriftClient<MyComposePostServiceClient>**) malloc(sizeof(MyThriftClient<MyComposePostServiceClient>**) * num_iterations);
    clients[i] = new MyThriftClient<MyUserServiceClient>(buffer_size);

    // for (int c = 0; c < num_iterations; c++) {
    //   clients[i][c] = new MyThriftClient<MyComposePostServiceClient>(buffer_size);
    // }

    processThreads[i] = std::thread(GenAndProcessReqs,
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
    delete clients[i];
    // for (int c = 0; c < num_iterations; c++) {
    //   delete clients[i][c];
    // }
  }

  // std::cout << "Percentage: " << (handler->counter * 100.0) / (num_iterations*num_threads) << std::endl;

  return 0;
}