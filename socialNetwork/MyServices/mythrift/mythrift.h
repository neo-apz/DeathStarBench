//
// Created by Arash on 23.10.18.
//

#ifndef BENCHMARKING_MYTHRIFT_H
#define BENCHMARKING_MYTHRIFT_H

//#include <stdlib.h>
#include <iostream>
#include "definitions.h"

namespace mythrift {

#define SIZE_SHIFT_MASK           24 // 3 bytes
#define NUM_ELEMENTS_SHIFT_MASK   32 // 4 bytes

#define TYPE_BYTE_MASK  0x00000000000000FF
#define ARRAY_TYPE_MASK  0x0000000000000080
#define SIZE_BYTE_MASK  0x00000000FF000000
#define NUM_ELEMENTS_MASK  0xFFFFFFFF00000000

enum WireType {
  WIRETYPE_VARINT           = 0,
  WIRETYPE_FIXED64          = 1,
  WIRETYPE_LENGTH_DELIMITED = 2,
  WIRETYPE_START_GROUP      = 3,
  WIRETYPE_END_GROUP        = 4,
  WIRETYPE_FIXED32          = 5,
};

enum FieldType {
  TYPE_DOUBLE         = 1,
  TYPE_FLOAT          = 2,

  TYPE_INT64          = 3,
  TYPE_UINT64         = 4,
  TYPE_INT32          = 5,

  TYPE_FIXED64        = 6,
  TYPE_FIXED32        = 7,

  TYPE_BOOL           = 8,

  TYPE_STRING         = 9,
  TYPE_GROUP          = 10,
  TYPE_MESSAGE        = 11,
  TYPE_BYTES          = 12,

  TYPE_UINT32         = 13,
  TYPE_ENUM           = 14,

  TYPE_SFIXED32       = 15,
  TYPE_SFIXED64       = 16,
  TYPE_SINT32         = 17,
  TYPE_SINT64         = 18,

  MAX_FIELD_TYPE      = 18,

  TYPE_ARRAY          = ARRAY_TYPE_MASK,

  MAX_LONG_NUM        = UINT64_MAX, // To make sure it's always 8 bytes!
};

enum Operation {
  SERIALIZATION   = 0,
  DESERIALIZATION = 1
};

typedef struct SchemaField {
  FieldType type;
  void* address;
  SchemaField() : type(MAX_FIELD_TYPE), address(NULL) {}
} SchemaField;

uint32 getWireType(FieldType fieldType);
uint64 getTypeSize(FieldType type);
uint64 getRepeatedType(FieldType type);
FieldType getRepeatedSingularType(uint64 type);
bool isRepeatedType(uint64 type);

}

#endif //BENCHMARKING_MYTHRIFT_H
