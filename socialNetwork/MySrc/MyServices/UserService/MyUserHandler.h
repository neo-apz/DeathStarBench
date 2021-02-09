#ifndef SOCIAL_NETWORK_MICROSERVICES_MYUSERHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_MYUSERHANDLER_H_

#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <chrono>
#include <mutex>

#include <vector>

#include "../../MyCommon/PicoSHA2/picosha2.h"

#include "../../gen-cpp/MyUserService.h"

#include "../../MyCommon/logger.h"

#include "../../MyCommon/RandomGenerator.h"

#define NUM_USERS 20
#define RAND_NUM_LIMIT 0xFFFFFFFFFFFFFF
#define NUM_ELEMENTS 10

// Custom Epoch (January 1, 2018 Midnight GMT = 2018-01-01T00:00:00Z)
#define CUSTOM_EPOCH 1514764800000

namespace my_social_network {
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

static int64_t current_timestamp = -1;
static int counter = 0;

static int GetCounter(int64_t timestamp) {
  if (current_timestamp > timestamp) {
    LOG(fatal) << "Timestamps are not incremental.";
    exit(EXIT_FAILURE);
  }
  if (current_timestamp == timestamp) {
    return counter++;
  } else {
    current_timestamp = timestamp;
    counter = 0;
    return counter++;
  }
}

std::string GenRandomString(const int len) {
  static const std::string alphanum =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(
      0, static_cast<int>(alphanum.length() - 1));
  std::string s;
  for (int i = 0; i < len; ++i) {
    s += alphanum[dist(gen)];
  }
  return s;
}


class MyUserHandler : public MyUserServiceIf {
 public:
  MyUserHandler(
      std::mutex*,
      const std::string &,
      const std::string &);
  ~MyUserHandler() override = default;

  int64_t RegisterUser(
      int64_t,
      const std::string &,
      const std::string &,
      const std::string &,
      const std::string &) override;
  int64_t RegisterUserWithId(
      int64_t,
      const std::string &,
      const std::string &,
      const std::string &,
      const std::string &,
      int64_t) override;

  int64_t UploadCreatorWithUserId(
      int64_t,
      int64_t,
      const std::string &) override;
  int64_t UploadCreatorWithUsername(
      int64_t,
      const std::string &) override;
  void Login(
      std::string &,
      int64_t,
      const std::string &,
      const std::string &) override;
  int64_t GetUserId(
      int64_t,
      const std::string &) override;

 private:
  std::string _machine_id;
  std::string _secret;
  std::mutex *_thread_lock;
  

  int64_t _users[NUM_USERS][NUM_USERS];

};

MyUserHandler::MyUserHandler(std::mutex *thread_lock,
                             const std::string &machine_id,
                             const std::string &secret) {
  
  _thread_lock = thread_lock;
  _machine_id = machine_id;
  _secret = secret;
}

// MyUserHandler::MyUserHandler() {

//   RandomGenerator randGen(1);

//   for (int i = 0; i < NUM_USERS; i++) {
//     for (int j = 0; j < NUM_USERS; j++) {
//       _users[i][j] = randGen.getUInt64(RAND_NUM_LIMIT);
//     }
//   }
// }


int64_t MyUserHandler::RegisterUserWithId(
    const int64_t req_id,
    const std::string &first_name,
    const std::string &last_name,
    const std::string &username,
    const std::string &password,
    const int64_t user_id) {

  User user;
  user.first_name = first_name;
  user.last_name = last_name;
  user.user_id = user_id;

  std::string salt = GenRandomString(32);
  user.password_hashed = picosha2::hash256_hex_string(password + salt);

  return user.user_id + user.password_hashed.length();
}

int64_t MyUserHandler::RegisterUser(
    const int64_t req_id,
    const std::string &first_name,
    const std::string &last_name,
    const std::string &username,
    const std::string &password) {

  // Compose user_id
  _thread_lock->lock();
  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count() - CUSTOM_EPOCH;
  int idx = GetCounter(timestamp);
  _thread_lock->unlock();

  std::stringstream sstream;
  sstream << std::hex << timestamp;
  std::string timestamp_hex(sstream.str());
  if (timestamp_hex.size() > 10) {
    timestamp_hex.erase(0, timestamp_hex.size() - 10);
  } else if (timestamp_hex.size() < 10) {
    timestamp_hex =
        std::string(10 - timestamp_hex.size(), '0') + timestamp_hex;
  }
  // Empty the sstream buffer.
  sstream.clear();
  sstream.str(std::string());

  sstream << std::hex << idx;
  std::string counter_hex(sstream.str());

  if (counter_hex.size() > 3) {
    counter_hex.erase(0, counter_hex.size() - 3);
  } else if (counter_hex.size() < 3) {
    counter_hex = std::string(3 - counter_hex.size(), '0') + counter_hex;
  }
  std::string user_id_str = _machine_id + timestamp_hex + counter_hex;
  int64_t user_id = stoul(user_id_str, nullptr, 16) & 0x7FFFFFFFFFFFFFFF;


  return this->RegisterUserWithId(req_id, first_name, last_name, username, password, user_id);
}

int64_t MyUserHandler::UploadCreatorWithUsername(
    const int64_t req_id,
    const std::string &username) {


  int64_t user_id = req_id + (req_id % 100);

  Creator creator;
  creator.username = username;
  creator.user_id = user_id;


  return creator.user_id + creator.username.length();

}

int64_t MyUserHandler::UploadCreatorWithUserId(
    int64_t req_id,
    int64_t user_id,
    const std::string &username) {

  Creator creator;
  creator.username = username;
  creator.user_id = user_id;


  return creator.user_id + creator.username.length();

}

void MyUserHandler::Login(
    std::string & _return,
    int64_t req_id,
    const std::string &username,
    const std::string &password) {

  std::string password_stored;
  std::string salt_stored;
  int64_t user_id_stored = -1;

  // json login_json;
  std::string login_json_str = "This is a test json string. It should be long."
                               "This is a test json string. It should be long."
                               "This is a test json string. It should be long."
                               "This is a test json string. It should be long.";

  // if (user_id_stored != -1 && !salt_stored.empty()
  //     && !password_stored.empty()) {
  //   bool auth = picosha2::hash256_hex_string(password + salt_stored)
  //       == password_stored;
  //   if (auth) {
  //     auto user_id_str = std::to_string(user_id_stored);
  //     auto timestamp_str = std::to_string(duration_cast<seconds>(
  //         system_clock::now().time_since_epoch()).count());
  //     jwt::jwt_object obj{
  //         algorithm("HS256"),
  //         secret(_secret),
  //         payload({
  //             {"user_id", user_id_str},
  //             {"username", username},
  //             {"timestamp", timestamp_str},
  //             {"ttl", "3600"}
  //         })
  //     };
  //     _return = obj.signature();

  //   } else {
  //     ServiceException se;
  //     se.errorCode = ErrorCode::SE_UNAUTHORIZED;
  //     se.message = "Incorrect username or password";
  //     throw se;
  //   }
  // } else {
  //   ServiceException se;
  //   se.errorCode = ErrorCode::SE_THRIFT_HANDLER_ERROR;
  //   se.message = "Username: " + username + " incomplete login information.";
  //   throw se;
  // }

  _return = login_json_str;
}

int64_t MyUserHandler::GetUserId(
    int64_t req_id,
    const std::string &username) {


  int64_t user_id = req_id + username.length();
  
  // span->Finish();
  return user_id;
}

/*
 * The following code which obtaines machine ID from machine's MAC address was
 * inspired from https://stackoverflow.com/a/16859693.
 */
u_int16_t HashMacAddressPid(const std::string &mac)
{
  u_int16_t hash = 0;
  std::string mac_pid = mac + std::to_string(getpid());
  for ( unsigned int i = 0; i < mac_pid.size(); i++ ) {
    hash += ( mac[i] << (( i & 1 ) * 8 ));
  }
  return hash;
}

int GetMachineId (std::string *mac_hash) {
  std::string mac;
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP );
  if ( sock < 0 ) {
    LOG(error) << "Unable to obtain MAC address";
    return -1;
  }

  struct ifconf conf{};
  char ifconfbuf[ 128 * sizeof(struct ifreq)  ];
  memset( ifconfbuf, 0, sizeof( ifconfbuf ));
  conf.ifc_buf = ifconfbuf;
  conf.ifc_len = sizeof( ifconfbuf );
  if ( ioctl( sock, SIOCGIFCONF, &conf ))
  {
    LOG(error) << "Unable to obtain MAC address";
    return -1;
  }

  struct ifreq* ifr;
  for (
      ifr = conf.ifc_req;
      reinterpret_cast<char *>(ifr) <
          reinterpret_cast<char *>(conf.ifc_req) + conf.ifc_len;
      ifr++) {
    if ( ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data ) {
      continue;  // duplicate, skip it
    }

    if ( ioctl( sock, SIOCGIFFLAGS, ifr )) {
      continue;  // failed to get flags, skip it
    }
    if ( ioctl( sock, SIOCGIFHWADDR, ifr ) == 0 ) {
      mac = std::string(ifr->ifr_addr.sa_data);
      if (!mac.empty()) {
        break;
      }
    }
  }
  close(sock);

  std::stringstream stream;
  stream << std::hex << HashMacAddressPid(mac);
  *mac_hash = stream.str();

  if (mac_hash->size() > 3) {
    mac_hash->erase(0, mac_hash->size() - 3);
  } else if (mac_hash->size() < 3) {
    *mac_hash = std::string(3 - mac_hash->size(), '0') + *mac_hash;
  }
  return 0;
}


} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_MYUSERHANDLER_H_
