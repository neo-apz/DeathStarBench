#include <cassert>
#include <algorithm>

#include "MyTMemoryBuffer.h"

using std::string;

namespace apache {
namespace thrift {
namespace transport {

void MyTMemoryBuffer::computeRead(uint32_t len, uint8_t** out_start, uint32_t* out_give) {
  // Correct rBound_ so we can use the fast path in the future.
  rBound_ = wBase_;

  // Decide how much to give.
  uint32_t give = (std::min)(len, available_read());

  *out_start = rBase_;
  *out_give = give;

  // Preincrement rBase_ so the caller doesn't have to.
  rBase_ += give;
}

uint32_t MyTMemoryBuffer::readSlow(uint8_t* buf, uint32_t len) {
  uint8_t* start;
  uint32_t give;
  computeRead(len, &start, &give);

  // Copy into the provided buffer.
  memcpy(buf, start, give);

  return give;
}

uint32_t MyTMemoryBuffer::readAppendToString(std::string& str, uint32_t len) {
  // Don't get some stupid assertion failure.
  if (buffer_ == nullptr) {
    return 0;
  }

  uint8_t* start;
  uint32_t give;
  computeRead(len, &start, &give);

  // Append to the provided string.
  str.append((char*)start, give);

  return give;
}

void MyTMemoryBuffer::ensureCanWrite(uint32_t len) {
  // Check available space
  uint32_t avail = available_write();
  if (len <= avail) {
    return;
  }

  if (!owner_) {
    throw TTransportException("Insufficient space in external MemoryBuffer");
  }

  // Grow the buffer as necessary.
  const uint32_t current_used = bufferSize_ - avail;
  const uint32_t required_buffer_size = len + current_used;
  if (required_buffer_size > maxBufferSize_) {
    throw TTransportException(TTransportException::BAD_ARGS,
                              "Internal buffer size overflow when requesting a buffer of size " + std::to_string(required_buffer_size));
  }

  // Always grow to the next bigger power of two:
  const double suggested_buffer_size = std::exp2(std::ceil(std::log2(required_buffer_size)));
  // Unless the power of two exceeds maxBufferSize_:
  const uint64_t new_size = static_cast<uint64_t>((std::min)(suggested_buffer_size, static_cast<double>(maxBufferSize_)));

  // Allocate into a new pointer so we don't bork ours if it fails.
  auto* new_buffer = static_cast<uint8_t*>(std::realloc(buffer_, static_cast<std::size_t>(new_size)));
  if (new_buffer == nullptr) {
    throw std::bad_alloc();
  }

  rBase_ = new_buffer + (rBase_ - buffer_);
  rBound_ = new_buffer + (rBound_ - buffer_);
  wBase_ = new_buffer + (wBase_ - buffer_);
  wBound_ = new_buffer + new_size;
  // Note: with realloc() we do not need to free the previous buffer:
  buffer_ = new_buffer;
  bufferSize_ = static_cast<uint32_t>(new_size);
}

void MyTMemoryBuffer::writeSlow(const uint8_t* buf, uint32_t len) {
  ensureCanWrite(len);

  // Copy into the buffer and increment wBase_.
  memcpy(wBase_, buf, len);
  wBase_ += len;
}

void MyTMemoryBuffer::wroteBytes(uint32_t len) {
  uint32_t avail = available_write();
  if (len > avail) {
    throw TTransportException("Client wrote more bytes than size of buffer.");
  }
  wBase_ += len;
}

const uint8_t* MyTMemoryBuffer::borrowSlow(uint8_t* buf, uint32_t* len) {
  (void)buf;
  rBound_ = wBase_;
  if (available_read() >= *len) {
    *len = available_read();
    return rBase_;
  }
  return nullptr;
}
}
}
} // apache::thrift::transport