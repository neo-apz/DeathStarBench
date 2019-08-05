//
// Created by Arash on 21.10.18.
//

#include <iostream>
#include <vector>
#include "mythrift.h"
#include "MyThriftMessage.h"
//#include "RepeatedField.h"

#if defined(SPARC) || defined(ARM)
#include "../flexus/flexus.h"
#endif


namespace mythrift {

// Returns number of bytes consumed.
// size_t MyThriftMessage::deserialize(byte *input, size_t size){

//   byte* remainingInput = input;
//   byte* limit = input + size;

//   currentField++; // Self
//   while (remainingInput < limit){
//     if (currentField > numFields) { // Error
//       return -1;
//     }

//     remainingInput = serializer->deserializeField(currentField, schema + currentField, remainingInput);

//     currentField++;
//   }

//   return remainingInput - input;
// }

// Returns number of bytes consumed.
// size_t MyThriftMessage::serialize(byte *target) {

//   size_t size = byteSize();

//   byte *newTarget = target;

//   for (uint32 i = 1; i <= numFields; ++i) {
//     newTarget += serializer->serializeField(i, schema + i, newTarget);
//   }

//   if (newTarget - target != size){
//     cout << "Error: calculated size (" << size <<
//             ") doesn't match the serialized version ("
//             << (newTarget - target) << ")!" << endl;

//     return -1;
//   }

//   return size;
// }

void MyThriftMessage::clear() {
  currentField = 0;
}

// size_t MyThriftMessage::byteSize() {
//   size_t total_size = this->getCachedSize();

//   if (total_size > 0)
//     // Already calculated and cached!
//     return total_size;

//   for (uint32 i = 1; i <= numFields; ++i) {
//     total_size += serializer->getFieldSize(i, schema + i);
//   }

//   // Set message size!
//   this->setCachedSize(total_size);

//   return total_size;
// }

uint64 MyThriftMessage::addField(uint64 type, void *address) {
  uint64 fieldID = currentField++;

  #if defined(FLEXUS)

  FieldType fieldType = (FieldType) type;

  type |= (getTypeSize(fieldType) << SIZE_SHIFT_MASK);

  if (getWireType(fieldType) == WIRETYPE_LENGTH_DELIMITED) {
    uint64 numElements = 0;

    if (fieldType == TYPE_STRING) {
      string &str = *static_cast<string *> (address);
      numElements = str.size();
      address = (void *) str.data();
    }

    else if (fieldType == TYPE_MESSAGE) {
      MyThriftMessage *msg = static_cast<MyThriftMessage *> (address);
      numElements = msg->numFields;
      address = (void *) (msg->schema + 1);
    }

    else if (isRepeatedType(fieldType)) {
      std::vector<void *> *v = static_cast<std::vector<void *>  *> (address);
      numElements = v->size();
      address = v->data();
    }

    type |= (numElements << NUM_ELEMENTS_SHIFT_MASK);
  }
  #endif

  schema[fieldID].type = static_cast<FieldType>(type);
  schema[fieldID].address = address;

  return fieldID;
}

}
