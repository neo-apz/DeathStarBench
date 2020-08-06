#ifndef MYCOMMON_RANDOMGENERATOR_H
#define MYCOMMON_RANDOMGENERATOR_H

#include <cstdlib>
#include <string>
#include <random>
#include <climits>
#include <algorithm>

using namespace std;

typedef unsigned char byte;

#define SEED0   0xBEEF
#define SEED1   0xDEAD

class RandomGenerator {

 public:

  RandomGenerator (unsigned short seed) {
    this->seed[0] = (unsigned short) (SEED0);
    this->seed[1] = (unsigned short) (SEED1);
    this->seed[0] = seed;
  }

  uint64_t getUInt64(uint64_t limit);
  uint64_t getUInt64(uint64_t begin, uint64_t end);

  int64_t getInt64(int64_t limit);
  int64_t getInt64(int64_t begin, int64_t end);

  uint32_t getUInt32(uint32_t limit);
  uint32_t getUInt32(uint32_t begin, uint32_t end);

  void getUInt32Set(uint32_t* set, size_t length);

  double getDouble(uint64_t limit);
  double getDouble(uint64_t begin, uint64_t end);

  float getFloat(uint64_t limit);
  float getFloat(uint64_t begin, uint64_t end);

  bool getBool();

  byte getbyte();


  string getAlphaString(size_t length);
  string getAlphaNumericString(size_t length);
  string getNumericString(size_t length);


 private:
  unsigned short seed[3];
  string randomString(const char *charset, size_t setSize, size_t length);

};

#endif //MYCOMMON_RANDOMGENERATOR_H