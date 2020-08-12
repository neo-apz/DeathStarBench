/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef FakeMediaService_H
#define FakeMediaService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "my_social_network_types.h"

namespace my_social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class FakeMediaServiceIf {
 public:
  virtual ~FakeMediaServiceIf() {}
  virtual void UploadMedia(const int64_t req_id, const std::vector<std::string> & media_types, const std::vector<int64_t> & media_ids) = 0;
};

class FakeMediaServiceIfFactory {
 public:
  typedef FakeMediaServiceIf Handler;

  virtual ~FakeMediaServiceIfFactory() {}

  virtual FakeMediaServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(FakeMediaServiceIf* /* handler */) = 0;
};

class FakeMediaServiceIfSingletonFactory : virtual public FakeMediaServiceIfFactory {
 public:
  FakeMediaServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<FakeMediaServiceIf>& iface) : iface_(iface) {}
  virtual ~FakeMediaServiceIfSingletonFactory() {}

  virtual FakeMediaServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(FakeMediaServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeMediaServiceIf> iface_;
};

class FakeMediaServiceNull : virtual public FakeMediaServiceIf {
 public:
  virtual ~FakeMediaServiceNull() {}
  void UploadMedia(const int64_t /* req_id */, const std::vector<std::string> & /* media_types */, const std::vector<int64_t> & /* media_ids */) {
    return;
  }
};

typedef struct _FakeMediaService_UploadMedia_args__isset {
  _FakeMediaService_UploadMedia_args__isset() : req_id(false), media_types(false), media_ids(false) {}
  bool req_id :1;
  bool media_types :1;
  bool media_ids :1;
} _FakeMediaService_UploadMedia_args__isset;

class FakeMediaService_UploadMedia_args {
 public:

  FakeMediaService_UploadMedia_args(const FakeMediaService_UploadMedia_args&);
  FakeMediaService_UploadMedia_args& operator=(const FakeMediaService_UploadMedia_args&);
  FakeMediaService_UploadMedia_args() : req_id(0) {
  }

  virtual ~FakeMediaService_UploadMedia_args() throw();
  int64_t req_id;
  std::vector<std::string>  media_types;
  std::vector<int64_t>  media_ids;

  _FakeMediaService_UploadMedia_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_media_types(const std::vector<std::string> & val);

  void __set_media_ids(const std::vector<int64_t> & val);

  bool operator == (const FakeMediaService_UploadMedia_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(media_types == rhs.media_types))
      return false;
    if (!(media_ids == rhs.media_ids))
      return false;
    return true;
  }
  bool operator != (const FakeMediaService_UploadMedia_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMediaService_UploadMedia_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMediaService_UploadMedia_pargs {
 public:


  virtual ~FakeMediaService_UploadMedia_pargs() throw();
  const int64_t* req_id;
  const std::vector<std::string> * media_types;
  const std::vector<int64_t> * media_ids;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMediaService_UploadMedia_result__isset {
  _FakeMediaService_UploadMedia_result__isset() : se(false) {}
  bool se :1;
} _FakeMediaService_UploadMedia_result__isset;

class FakeMediaService_UploadMedia_result {
 public:

  FakeMediaService_UploadMedia_result(const FakeMediaService_UploadMedia_result&);
  FakeMediaService_UploadMedia_result& operator=(const FakeMediaService_UploadMedia_result&);
  FakeMediaService_UploadMedia_result() {
  }

  virtual ~FakeMediaService_UploadMedia_result() throw();
  ServiceException se;

  _FakeMediaService_UploadMedia_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeMediaService_UploadMedia_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeMediaService_UploadMedia_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMediaService_UploadMedia_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMediaService_UploadMedia_presult__isset {
  _FakeMediaService_UploadMedia_presult__isset() : se(false) {}
  bool se :1;
} _FakeMediaService_UploadMedia_presult__isset;

class FakeMediaService_UploadMedia_presult {
 public:


  virtual ~FakeMediaService_UploadMedia_presult() throw();
  ServiceException se;

  _FakeMediaService_UploadMedia_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class FakeMediaServiceClient : virtual public FakeMediaServiceIf {
 public:
  FakeMediaServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FakeMediaServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void UploadMedia(const int64_t req_id, const std::vector<std::string> & media_types, const std::vector<int64_t> & media_ids);
  void send_UploadMedia(const int64_t req_id, const std::vector<std::string> & media_types, const std::vector<int64_t> & media_ids);
  void recv_UploadMedia();
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class FakeMediaServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeMediaServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (FakeMediaServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_UploadMedia(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  FakeMediaServiceProcessor(::apache::thrift::stdcxx::shared_ptr<FakeMediaServiceIf> iface) :
    iface_(iface) {
    processMap_["UploadMedia"] = &FakeMediaServiceProcessor::process_UploadMedia;
  }

  virtual ~FakeMediaServiceProcessor() {}
};

class FakeMediaServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  FakeMediaServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< FakeMediaServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< FakeMediaServiceIfFactory > handlerFactory_;
};

class FakeMediaServiceMultiface : virtual public FakeMediaServiceIf {
 public:
  FakeMediaServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<FakeMediaServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~FakeMediaServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<FakeMediaServiceIf> > ifaces_;
  FakeMediaServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<FakeMediaServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void UploadMedia(const int64_t req_id, const std::vector<std::string> & media_types, const std::vector<int64_t> & media_ids) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadMedia(req_id, media_types, media_ids);
    }
    ifaces_[i]->UploadMedia(req_id, media_types, media_ids);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class FakeMediaServiceConcurrentClient : virtual public FakeMediaServiceIf {
 public:
  FakeMediaServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FakeMediaServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void UploadMedia(const int64_t req_id, const std::vector<std::string> & media_types, const std::vector<int64_t> & media_ids);
  int32_t send_UploadMedia(const int64_t req_id, const std::vector<std::string> & media_types, const std::vector<int64_t> & media_ids);
  void recv_UploadMedia(const int32_t seqid);
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