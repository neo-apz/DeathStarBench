/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef MyUniqueIdService_H
#define MyUniqueIdService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "social_network_types.h"

namespace social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class MyUniqueIdServiceIf {
 public:
  virtual ~MyUniqueIdServiceIf() {}
  virtual void UploadUniqueId(const int64_t req_id, const PostType::type post_type) = 0;
};

class MyUniqueIdServiceIfFactory {
 public:
  typedef MyUniqueIdServiceIf Handler;

  virtual ~MyUniqueIdServiceIfFactory() {}

  virtual MyUniqueIdServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(MyUniqueIdServiceIf* /* handler */) = 0;
};

class MyUniqueIdServiceIfSingletonFactory : virtual public MyUniqueIdServiceIfFactory {
 public:
  MyUniqueIdServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf>& iface) : iface_(iface) {}
  virtual ~MyUniqueIdServiceIfSingletonFactory() {}

  virtual MyUniqueIdServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(MyUniqueIdServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface_;
};

class MyUniqueIdServiceNull : virtual public MyUniqueIdServiceIf {
 public:
  virtual ~MyUniqueIdServiceNull() {}
  void UploadUniqueId(const int64_t /* req_id */, const PostType::type /* post_type */) {
    return;
  }
};

typedef struct _MyUniqueIdService_UploadUniqueId_args__isset {
  _MyUniqueIdService_UploadUniqueId_args__isset() : req_id(false), post_type(false) {}
  bool req_id :1;
  bool post_type :1;
} _MyUniqueIdService_UploadUniqueId_args__isset;

class MyUniqueIdService_UploadUniqueId_args {
 public:

  MyUniqueIdService_UploadUniqueId_args(const MyUniqueIdService_UploadUniqueId_args&);
  MyUniqueIdService_UploadUniqueId_args& operator=(const MyUniqueIdService_UploadUniqueId_args&);
  MyUniqueIdService_UploadUniqueId_args() : req_id(0), post_type((PostType::type)0) {
  }

  virtual ~MyUniqueIdService_UploadUniqueId_args() throw();
  int64_t req_id;
  PostType::type post_type;

  _MyUniqueIdService_UploadUniqueId_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post_type(const PostType::type val);

  bool operator == (const MyUniqueIdService_UploadUniqueId_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post_type == rhs.post_type))
      return false;
    return true;
  }
  bool operator != (const MyUniqueIdService_UploadUniqueId_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyUniqueIdService_UploadUniqueId_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MyUniqueIdService_UploadUniqueId_pargs {
 public:


  virtual ~MyUniqueIdService_UploadUniqueId_pargs() throw();
  const int64_t* req_id;
  const PostType::type* post_type;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyUniqueIdService_UploadUniqueId_result__isset {
  _MyUniqueIdService_UploadUniqueId_result__isset() : se(false) {}
  bool se :1;
} _MyUniqueIdService_UploadUniqueId_result__isset;

class MyUniqueIdService_UploadUniqueId_result {
 public:

  MyUniqueIdService_UploadUniqueId_result(const MyUniqueIdService_UploadUniqueId_result&);
  MyUniqueIdService_UploadUniqueId_result& operator=(const MyUniqueIdService_UploadUniqueId_result&);
  MyUniqueIdService_UploadUniqueId_result() {
  }

  virtual ~MyUniqueIdService_UploadUniqueId_result() throw();
  ServiceException se;

  _MyUniqueIdService_UploadUniqueId_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const MyUniqueIdService_UploadUniqueId_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const MyUniqueIdService_UploadUniqueId_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyUniqueIdService_UploadUniqueId_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyUniqueIdService_UploadUniqueId_presult__isset {
  _MyUniqueIdService_UploadUniqueId_presult__isset() : se(false) {}
  bool se :1;
} _MyUniqueIdService_UploadUniqueId_presult__isset;

class MyUniqueIdService_UploadUniqueId_presult {
 public:


  virtual ~MyUniqueIdService_UploadUniqueId_presult() throw();
  ServiceException se;

  _MyUniqueIdService_UploadUniqueId_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MyUniqueIdServiceClient : virtual public MyUniqueIdServiceIf {
 public:
  MyUniqueIdServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MyUniqueIdServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  void send_UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  void recv_UploadUniqueId();
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class MyUniqueIdServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MyUniqueIdServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_UploadUniqueId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  MyUniqueIdServiceProcessor(::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface) :
    iface_(iface) {
    processMap_["UploadUniqueId"] = &MyUniqueIdServiceProcessor::process_UploadUniqueId;
  }

  virtual ~MyUniqueIdServiceProcessor() {}
};

class MyUniqueIdServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  MyUniqueIdServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< MyUniqueIdServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< MyUniqueIdServiceIfFactory > handlerFactory_;
};

class MyUniqueIdServiceMultiface : virtual public MyUniqueIdServiceIf {
 public:
  MyUniqueIdServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MyUniqueIdServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> > ifaces_;
  MyUniqueIdServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<MyUniqueIdServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void UploadUniqueId(const int64_t req_id, const PostType::type post_type) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUniqueId(req_id, post_type);
    }
    ifaces_[i]->UploadUniqueId(req_id, post_type);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class MyUniqueIdServiceConcurrentClient : virtual public MyUniqueIdServiceIf {
 public:
  MyUniqueIdServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MyUniqueIdServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  int32_t send_UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  void recv_UploadUniqueId(const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

} // namespace

#endif
