//
// Created by Arash on 09.11.18.
//

#include "mythrift.h"

namespace mythrift {

uint32 getWireType(FieldType fieldType) {

  if (isRepeatedType(fieldType)) return WIRETYPE_LENGTH_DELIMITED;

  uint32 wireType = 0;

  switch (fieldType) {
    case TYPE_INT32:
    case TYPE_INT64:
    case TYPE_UINT32:
    case TYPE_UINT64:
    case TYPE_SINT32:
    case TYPE_SINT64:
    case TYPE_BOOL:
    case TYPE_ENUM:
      wireType = WIRETYPE_VARINT;
      break;

    case TYPE_FIXED64:
    case TYPE_SFIXED64:
    case TYPE_DOUBLE:
      wireType = WIRETYPE_FIXED64;
      break;

    case TYPE_STRING:
    case TYPE_BYTES:
    case TYPE_MESSAGE:
    case TYPE_ARRAY:
      wireType = WIRETYPE_LENGTH_DELIMITED;
      break;

    case TYPE_GROUP:
      wireType = WIRETYPE_START_GROUP;
      break;

    case TYPE_FIXED32:
    case TYPE_SFIXED32:
    case TYPE_FLOAT:
      wireType = WIRETYPE_FIXED32;
      break;

    case MAX_LONG_NUM:
      break;
  }

  return wireType;
}

uint64 getTypeSize(FieldType type){

  if (isRepeatedType(type)) return getTypeSize(getRepeatedSingularType(type)); // Size of the singular type

  switch (type) {
    case TYPE_MESSAGE:
      return sizeof(SchemaField); // 2 x sizeof(uint64)

    case TYPE_ENUM: // Treat enum as int32!
    case TYPE_INT32:
    case TYPE_UINT32:
    case TYPE_SFIXED32:
    case TYPE_FIXED32:
    case TYPE_SINT32:
      return sizeof(uint32);

    case TYPE_INT64: // Uint64 and int64 have the same size!
    case TYPE_UINT64:
    case TYPE_SFIXED64:
    case TYPE_FIXED64:
    case TYPE_SINT64:
      return sizeof(uint64);

    case TYPE_BYTES: // Treat byte[] as string!
    case TYPE_STRING:
      return sizeof(char);

    case TYPE_BOOL:
      return sizeof(bool);

    case TYPE_DOUBLE:
      return sizeof(double);

    case TYPE_FLOAT:
      return sizeof(float);

      // We don't implement this!
    case TYPE_GROUP:
    case MAX_LONG_NUM:
    case TYPE_ARRAY:
      return 0;
  }

  return 0;
}

uint64 getRepeatedType(FieldType type){
  return (type | ARRAY_TYPE_MASK);
}

FieldType getRepeatedSingularType(uint64 type) {
  return static_cast<FieldType>(type ^ ARRAY_TYPE_MASK);
}

bool isRepeatedType(uint64 type) {
  return ((type & ARRAY_TYPE_MASK) == ARRAY_TYPE_MASK);
}

}
