//
// Created by Arash on 07.11.18.
//

#include "RandomGenerator.h"

// [0, limit)
uint64_t RandomGenerator::getUInt64(uint64_t limit) {
  return ((uint64_t) getDouble(limit));
}

uint64_t RandomGenerator::getUInt64(uint64_t begin, uint64_t end) {
  return getUInt64(end - begin + 1) + begin;
}

uint32_t RandomGenerator::getUInt32(uint32_t limit) {
  return ((uint32_t) getDouble(limit));
}

int64_t RandomGenerator::getInt64(int64_t limit) {
    return ((int64_t) getDouble(limit));
}

int64_t RandomGenerator::getInt64(int64_t begin, int64_t end) {
    return getInt64(end - begin + 1) + begin;
}


uint32_t RandomGenerator::getUInt32(uint32_t begin, uint32_t end) {
  return getUInt32(end - begin + 1) + begin;
}

// random[length] --> [0, length)
void RandomGenerator::getUInt32Set(uint32_t* set, size_t length) {
  for(int i = 0; i < length; i++){
    set[i] = i;
  }

  for(int i = 0; i < length; i++){
    swap(set[i], set[getUInt32(length)]);
  }
}

string RandomGenerator::randomString(const char *charset, size_t setSize, size_t length) {
  auto randchar = [charset, setSize, this]() -> char
  {
    return charset[ this->getUInt32(setSize - 1) ];
  };
  string str(length,0);
  generate_n( str.begin(), length, randchar );
  return str;
}

string RandomGenerator::getAlphaString(size_t length) {
  const char charset[] = "abcdefghijklmnopqrstuvwxyz";
  return this->randomString(charset, sizeof(charset), length);
}

string RandomGenerator::getAlphaNumericString(size_t length) {
  const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
  return this->randomString(charset, sizeof(charset), length);
}

string RandomGenerator::getNumericString(size_t length) {
  const char charset[] = "0123456789";
  return this->randomString(charset, sizeof(charset), length);
}

double RandomGenerator::getDouble(uint64_t limit) {
  return erand48(this->seed) * limit;
}

double RandomGenerator::getDouble(uint64_t begin, uint64_t end) {
  return getDouble(end - begin + 1) + begin;
}

float RandomGenerator::getFloat(uint64_t limit) {
  return (float) getDouble(limit);
}

float RandomGenerator::getFloat(uint64_t begin, uint64_t end) {
  return getFloat(end - begin + 1) + begin;
}

bool RandomGenerator::getBool() {
  return (getFloat(1) > 0.5);
}

byte RandomGenerator::getbyte() {
  return (byte)(((byte) getDouble(CHAR_MAX)));
}