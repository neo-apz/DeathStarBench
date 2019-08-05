//
// Created by Arash on 21.10.18.
//

#ifndef BENCHMARKING_MYTHRIFTMESSAGE_H
#define BENCHMARKING_MYTHRIFTMESSAGE_H

#include <string>
#include "mythrift.h"
// #include "ProtobufSerializer.h"

using namespace std;

namespace mythrift {

class MyThriftMessage {

 public:

  SchemaField* getSchema() const {
    return schema;
  }

  void setSchema(SchemaField *schema) {
    MyThriftMessage::schema = schema;
  }

  // size_t getCachedSize() const {
  //   return cachedSize;
  // }
  // void setCachedSize(size_t cachedSize) {
  //   MyThriftMessage::cachedSize = cachedSize;
  // }

  uint64 getNumFields() const {
    return numFields;
  }

  // size_t byteSize();
  // size_t deserialize(byte *input, size_t size);
  // size_t serialize(byte *target);
  void clear();

  SchemaField* schema;
  uint64 numFields = 0;
  uint64 currentField = 0;
  size_t cachedSize = 0;

  // ProtobufSerializer* serializer = ProtobufSerializer::getSerializer();

  uint64 addField(uint64 type, void *address);

};

}

#endif //BENCHMARKING_MYTHRIFTMESSAGE_H
