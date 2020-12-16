#ifndef SOCIAL_NETWORK_MICROSERVICES_MYSOCIALGRAPHHANDLER_H_
#define SOCIAL_NETWORK_MICROSERVICES_MYSOCIALGRAPHHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include <mutex>

#include "../../gen-cpp/MySocialGraphService.h"

#include "../../MyCommon/logger.h"

#include "../../MyCommon/RandomGenerator.h"

#define NUM_USERS 20

#define RAND_NUM_LIMIT 0xFFFFFFFFFFFFFF

#define NUM_ELEMENTS 10
namespace my_social_network {
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

class MySocialGraphHandler : public MySocialGraphServiceIf {
 public:
  MySocialGraphHandler();
  ~MySocialGraphHandler() override = default;

  // uint64_t counter = 0;
  // std::mutex _counter_lock;

  void GetFollowers(std::vector<int64_t> &, int64_t, int64_t) override;
  void GetFollowees(std::vector<int64_t> &, int64_t, int64_t) override;
  int64_t Follow(int64_t, int64_t, int64_t) override;
  int64_t Unfollow(int64_t, int64_t, int64_t) override;
  int64_t FollowWithUsername(int64_t, const std::string &, const std::string &) override;
  int64_t UnfollowWithUsername(int64_t, const std::string &, const std::string &) override;
  int64_t InsertUser(int64_t, int64_t) override;

 private:
  int64_t _users[NUM_USERS][NUM_USERS];

};

MySocialGraphHandler::MySocialGraphHandler() {

  RandomGenerator randGen(1);

  for (int i = 0; i < NUM_USERS; i++) {
    for (int j = 0; j < NUM_USERS; j++) {
      _users[i][j] = randGen.getUInt64(RAND_NUM_LIMIT);
    }
  }
}

void MySocialGraphHandler::GetFollowers(std::vector<int64_t> & _return,
                           const int64_t req_id, const int64_t user_id) {

  int i = user_id % NUM_USERS;

  int count = req_id % (NUM_USERS - 1) + 1;

  for (int j = 0; j < count; j++) {
      _return.push_back(_users[i][j]);
  }

  for (int j = 0; j < count; j++) {
      _return.push_back(_users[i][j]);
  }
}

void MySocialGraphHandler::GetFollowees(std::vector<int64_t> & _return,
                           const int64_t req_id, const int64_t user_id) {

  int i = user_id % NUM_USERS;

  int count = req_id % (NUM_USERS - 1) + 1;

  for (int j = 0; j < count; j++) {
      _return.push_back(_users[j][i]);
  }

  for (int j = 0; j < count; j++) {
      _return.push_back(_users[j][i]);
  }
}

int64_t MySocialGraphHandler::Follow(
    int64_t req_id,
    int64_t user_id,
    int64_t followee_id) {

  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count();

  int i = user_id % NUM_USERS;
  int j = followee_id % NUM_USERS;

  _users[i][j] = _users[i][j] + followee_id + timestamp;
  _users[j][i] = _users[j][i] + user_id + timestamp;

  return _users[i][j];

}


int64_t MySocialGraphHandler::Unfollow(
    int64_t req_id,
    int64_t user_id,
    int64_t followee_id) {

  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count();

  int i = user_id % NUM_USERS;
  int j = followee_id % NUM_USERS;

  _users[i][j] = _users[i][j] - followee_id + timestamp;
  _users[j][i] = _users[j][i] - user_id + timestamp;

  return _users[i][j];
}

int64_t MySocialGraphHandler::FollowWithUsername(
    int64_t req_id,
    const std::string &user_name,
    const std::string &followee_name) {

  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count();

  int64_t user_id = ((int64_t) user_name.at(timestamp % 10)) % NUM_USERS;
  int64_t followee_id = ((int64_t) followee_name.at(timestamp % 10)) % NUM_USERS;

  return Follow(req_id, user_id, followee_id);
}

int64_t MySocialGraphHandler::UnfollowWithUsername(
    int64_t req_id,
    const std::string &user_name,
    const std::string &followee_name) {

  int64_t timestamp = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count();

  int64_t user_id = ((int64_t) user_name.at(timestamp % 10) + user_name.at(timestamp % 5)) % NUM_USERS;
  int64_t followee_id = ((int64_t) followee_name.at(timestamp % 10) + followee_name.at(timestamp % 5)) % NUM_USERS;

  return Unfollow(req_id, user_id, followee_id);
}

int64_t MySocialGraphHandler::InsertUser(int64_t req_id, int64_t user_id) {
  return req_id + user_id;
}

} // namespace my_social_network

#endif //SOCIAL_NETWORK_MICROSERVICES_MYSOCIALGRAPHHANDLER_H_
