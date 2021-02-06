/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "UniqueIdService.h"

namespace my_social_network {


UniqueIdService_UploadUniqueId_args::~UniqueIdService_UploadUniqueId_args() throw() {
}


uint32_t UniqueIdService_UploadUniqueId_args::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->req_id);
          this->__isset.req_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast31;
          xfer += iprot->readI32(ecast31);
          this->post_type = (PostType::type)ecast31;
          this->__isset.post_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t UniqueIdService_UploadUniqueId_args::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("UniqueIdService_UploadUniqueId_args");

  xfer += oprot->writeFieldBegin("req_id", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->req_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("post_type", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32((int32_t)this->post_type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}


UniqueIdService_UploadUniqueId_pargs::~UniqueIdService_UploadUniqueId_pargs() throw() {
}


uint32_t UniqueIdService_UploadUniqueId_pargs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("UniqueIdService_UploadUniqueId_pargs");

  xfer += oprot->writeFieldBegin("req_id", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64((*(this->req_id)));
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("post_type", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32((int32_t)(*(this->post_type)));
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}


UniqueIdService_UploadUniqueId_result::~UniqueIdService_UploadUniqueId_result() throw() {
}


uint32_t UniqueIdService_UploadUniqueId_result::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->success);
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t UniqueIdService_UploadUniqueId_result::write(::apache::thrift::protocol::TProtocol* oprot) const {

  uint32_t xfer = 0;

  xfer += oprot->writeStructBegin("UniqueIdService_UploadUniqueId_result");

  if (this->__isset.success) {
    xfer += oprot->writeFieldBegin("success", ::apache::thrift::protocol::T_I64, 0);
    xfer += oprot->writeI64(this->success);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}


UniqueIdService_UploadUniqueId_presult::~UniqueIdService_UploadUniqueId_presult() throw() {
}


uint32_t UniqueIdService_UploadUniqueId_presult::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 0:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64((*(this->success)));
          this->__isset.success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

void UniqueIdServiceClient::initArgs(RandomGenerator* randGen)
{
	this->uploadUniqueId_args = new UniqueIdService_UploadUniqueId_args(randGen);
}

void UniqueIdServiceClient::send_RandReq(RandomGenerator* randGen)
{
	send_UploadUniqueId(uploadUniqueId_args->req_id, uploadUniqueId_args->post_type);
}

void UniqueIdServiceClient::initResults(RandomGenerator* randGen)
{
	this->uploadUniqueId_res = new UniqueIdService_UploadUniqueId_result(randGen);
}

int64_t UniqueIdServiceClient::UploadUniqueId(const int64_t req_id, const PostType::type post_type)
{
  send_UploadUniqueId(req_id, post_type);
  return recv_UploadUniqueId();
}

void UniqueIdServiceClient::send_UploadUniqueId(const int64_t req_id, const PostType::type post_type)
{
  int32_t cseqid = 0;
  oprot_->writeMessageBegin("UploadUniqueId", ::apache::thrift::protocol::T_CALL, cseqid);

  UniqueIdService_UploadUniqueId_pargs args;
  args.req_id = &req_id;
  args.post_type = &post_type;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->writeEnd();
  oprot_->getTransport()->flush();
}

int64_t UniqueIdServiceClient::recv_UploadUniqueId()
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  iprot_->readMessageBegin(fname, mtype, rseqid);
  if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
    ::apache::thrift::TApplicationException x;
    x.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
    throw x;
  }
  if (mtype != ::apache::thrift::protocol::T_REPLY) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  if (fname.compare("UploadUniqueId") != 0) {
    iprot_->skip(::apache::thrift::protocol::T_STRUCT);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }
  int64_t _return;
  UniqueIdService_UploadUniqueId_presult result;
  result.success = &_return;
  result.read(iprot_);
  iprot_->readMessageEnd();
  iprot_->getTransport()->readEnd();

  if (result.__isset.success) {
    return _return;
  }
  throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::MISSING_RESULT, "UploadUniqueId failed: unknown result");
}

bool UniqueIdServiceProcessor::dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext) {
  ProcessMap::iterator pfn;
  pfn = processMap_.find(fname);
  if (pfn == processMap_.end()) {
    iprot->skip(::apache::thrift::protocol::T_STRUCT);
    iprot->readMessageEnd();
    iprot->getTransport()->readEnd();
    ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::UNKNOWN_METHOD, "Invalid method name: '"+fname+"'");
    oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return true;
  }
  (this->*(pfn->second))(seqid, iprot, oprot, callContext);
  return true;
}

void UniqueIdServiceProcessor::process_UploadUniqueId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext)
{
  void* ctx = NULL;
  if (this->eventHandler_.get() != NULL) {
    ctx = this->eventHandler_->getContext("UniqueIdService.UploadUniqueId", callContext);
  }
  ::apache::thrift::TProcessorContextFreer freer(this->eventHandler_.get(), ctx, "UniqueIdService.UploadUniqueId");

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preRead(ctx, "UniqueIdService.UploadUniqueId");
  }

  UniqueIdService_UploadUniqueId_args args;
  args.read(iprot);
  iprot->readMessageEnd();
  uint32_t bytes = iprot->getTransport()->readEnd();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postRead(ctx, "UniqueIdService.UploadUniqueId", bytes);
  }

  UniqueIdService_UploadUniqueId_result result;
  try {
    result.success = iface_->UploadUniqueId(args.req_id, args.post_type);
    result.__isset.success = true;
  } catch (const std::exception& e) {
    if (this->eventHandler_.get() != NULL) {
      this->eventHandler_->handlerError(ctx, "UniqueIdService.UploadUniqueId");
    }

    ::apache::thrift::TApplicationException x(e.what());
    oprot->writeMessageBegin("UploadUniqueId", ::apache::thrift::protocol::T_EXCEPTION, seqid);
    x.write(oprot);
    oprot->writeMessageEnd();
    oprot->getTransport()->writeEnd();
    oprot->getTransport()->flush();
    return;
  }

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->preWrite(ctx, "UniqueIdService.UploadUniqueId");
  }

  oprot->writeMessageBegin("UploadUniqueId", ::apache::thrift::protocol::T_REPLY, seqid);
  result.write(oprot);
  oprot->writeMessageEnd();
  bytes = oprot->getTransport()->writeEnd();
  oprot->getTransport()->flush();

  if (this->eventHandler_.get() != NULL) {
    this->eventHandler_->postWrite(ctx, "UniqueIdService.UploadUniqueId", bytes);
  }
}

uint64_t UniqueIdServiceCerebrosProcessor::process_UploadUniqueId(UniqueIdServiceClient* client) {
	auto args = client->uploadUniqueId_args;
	auto res = client->uploadUniqueId_res;

	res->success = iface_->UploadUniqueId(args->req_id, args->post_type);

	return (uint64_t) res;
}

::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > UniqueIdServiceProcessorFactory::getProcessor(const ::apache::thrift::TConnectionInfo& connInfo) {
  ::apache::thrift::ReleaseHandler< UniqueIdServiceIfFactory > cleanup(handlerFactory_);
  ::apache::thrift::stdcxx::shared_ptr< UniqueIdServiceIf > handler(handlerFactory_->getHandler(connInfo), cleanup);
  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > processor(new UniqueIdServiceProcessor(handler));
  return processor;
}

int64_t UniqueIdServiceConcurrentClient::UploadUniqueId(const int64_t req_id, const PostType::type post_type)
{
  int32_t seqid = send_UploadUniqueId(req_id, post_type);
  return recv_UploadUniqueId(seqid);
}

int32_t UniqueIdServiceConcurrentClient::send_UploadUniqueId(const int64_t req_id, const PostType::type post_type)
{
  int32_t cseqid = this->sync_.generateSeqId();
  ::apache::thrift::async::TConcurrentSendSentry sentry(&this->sync_);
  oprot_->writeMessageBegin("UploadUniqueId", ::apache::thrift::protocol::T_CALL, cseqid);

  UniqueIdService_UploadUniqueId_pargs args;
  args.req_id = &req_id;
  args.post_type = &post_type;
  args.write(oprot_);

  oprot_->writeMessageEnd();
  oprot_->getTransport()->writeEnd();
  oprot_->getTransport()->flush();

  sentry.commit();
  return cseqid;
}

int64_t UniqueIdServiceConcurrentClient::recv_UploadUniqueId(const int32_t seqid)
{

  int32_t rseqid = 0;
  std::string fname;
  ::apache::thrift::protocol::TMessageType mtype;

  // the read mutex gets dropped and reacquired as part of waitForWork()
  // The destructor of this sentry wakes up other clients
  ::apache::thrift::async::TConcurrentRecvSentry sentry(&this->sync_, seqid);

  while(true) {
    if(!this->sync_.getPending(fname, mtype, rseqid)) {
      iprot_->readMessageBegin(fname, mtype, rseqid);
    }
    if(seqid == rseqid) {
      if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
        ::apache::thrift::TApplicationException x;
        x.read(iprot_);
        iprot_->readMessageEnd();
        iprot_->getTransport()->readEnd();
        sentry.commit();
        throw x;
      }
      if (mtype != ::apache::thrift::protocol::T_REPLY) {
        iprot_->skip(::apache::thrift::protocol::T_STRUCT);
        iprot_->readMessageEnd();
        iprot_->getTransport()->readEnd();
      }
      if (fname.compare("UploadUniqueId") != 0) {
        iprot_->skip(::apache::thrift::protocol::T_STRUCT);
        iprot_->readMessageEnd();
        iprot_->getTransport()->readEnd();

        // in a bad state, don't commit
        using ::apache::thrift::protocol::TProtocolException;
        throw TProtocolException(TProtocolException::INVALID_DATA);
      }
      int64_t _return;
      UniqueIdService_UploadUniqueId_presult result;
      result.success = &_return;
      result.read(iprot_);
      iprot_->readMessageEnd();
      iprot_->getTransport()->readEnd();

      if (result.__isset.success) {
        sentry.commit();
        return _return;
      }
      // in a bad state, don't commit
      throw ::apache::thrift::TApplicationException(::apache::thrift::TApplicationException::MISSING_RESULT, "UploadUniqueId failed: unknown result");
    }
    // seqid != rseqid
    this->sync_.updatePending(fname, mtype, rseqid);

    // this will temporarily unlock the readMutex, and let other clients get work done
    this->sync_.waitForWork(seqid);
  } // end while(true)
}

} // namespace
