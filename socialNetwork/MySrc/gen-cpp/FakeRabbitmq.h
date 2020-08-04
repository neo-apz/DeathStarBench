/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef FakeRabbitmq_H
#define FakeRabbitmq_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "my_social_network_types.h"

namespace my_social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class FakeRabbitmqIf {
 public:
  virtual ~FakeRabbitmqIf() {}
  virtual void UploadHomeTimeline(const int64_t req_id, const int64_t post_id, const int64_t user_id, const int64_t timestamp, const std::vector<int64_t> & user_mentions_id) = 0;
};

class FakeRabbitmqIfFactory {
 public:
  typedef FakeRabbitmqIf Handler;

  virtual ~FakeRabbitmqIfFactory() {}

  virtual FakeRabbitmqIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(FakeRabbitmqIf* /* handler */) = 0;
};

class FakeRabbitmqIfSingletonFactory : virtual public FakeRabbitmqIfFactory {
 public:
  FakeRabbitmqIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<FakeRabbitmqIf>& iface) : iface_(iface) {}
  virtual ~FakeRabbitmqIfSingletonFactory() {}

  virtual FakeRabbitmqIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(FakeRabbitmqIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeRabbitmqIf> iface_;
};

class FakeRabbitmqNull : virtual public FakeRabbitmqIf {
 public:
  virtual ~FakeRabbitmqNull() {}
  void UploadHomeTimeline(const int64_t /* req_id */, const int64_t /* post_id */, const int64_t /* user_id */, const int64_t /* timestamp */, const std::vector<int64_t> & /* user_mentions_id */) {
    return;
  }
};

typedef struct _FakeRabbitmq_UploadHomeTimeline_args__isset {
  _FakeRabbitmq_UploadHomeTimeline_args__isset() : req_id(false), post_id(false), user_id(false), timestamp(false), user_mentions_id(false) {}
  bool req_id :1;
  bool post_id :1;
  bool user_id :1;
  bool timestamp :1;
  bool user_mentions_id :1;
} _FakeRabbitmq_UploadHomeTimeline_args__isset;

class FakeRabbitmq_UploadHomeTimeline_args {
 public:

  FakeRabbitmq_UploadHomeTimeline_args(const FakeRabbitmq_UploadHomeTimeline_args&);
  FakeRabbitmq_UploadHomeTimeline_args& operator=(const FakeRabbitmq_UploadHomeTimeline_args&);
  FakeRabbitmq_UploadHomeTimeline_args() : req_id(0), post_id(0), user_id(0), timestamp(0) {
  }

  virtual ~FakeRabbitmq_UploadHomeTimeline_args() throw();
  int64_t req_id;
  int64_t post_id;
  int64_t user_id;
  int64_t timestamp;
  std::vector<int64_t>  user_mentions_id;

  _FakeRabbitmq_UploadHomeTimeline_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post_id(const int64_t val);

  void __set_user_id(const int64_t val);

  void __set_timestamp(const int64_t val);

  void __set_user_mentions_id(const std::vector<int64_t> & val);

  bool operator == (const FakeRabbitmq_UploadHomeTimeline_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post_id == rhs.post_id))
      return false;
    if (!(user_id == rhs.user_id))
      return false;
    if (!(timestamp == rhs.timestamp))
      return false;
    if (!(user_mentions_id == rhs.user_mentions_id))
      return false;
    return true;
  }
  bool operator != (const FakeRabbitmq_UploadHomeTimeline_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeRabbitmq_UploadHomeTimeline_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeRabbitmq_UploadHomeTimeline_pargs {
 public:


  virtual ~FakeRabbitmq_UploadHomeTimeline_pargs() throw();
  const int64_t* req_id;
  const int64_t* post_id;
  const int64_t* user_id;
  const int64_t* timestamp;
  const std::vector<int64_t> * user_mentions_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeRabbitmq_UploadHomeTimeline_result__isset {
  _FakeRabbitmq_UploadHomeTimeline_result__isset() : se(false) {}
  bool se :1;
} _FakeRabbitmq_UploadHomeTimeline_result__isset;

class FakeRabbitmq_UploadHomeTimeline_result {
 public:

  FakeRabbitmq_UploadHomeTimeline_result(const FakeRabbitmq_UploadHomeTimeline_result&);
  FakeRabbitmq_UploadHomeTimeline_result& operator=(const FakeRabbitmq_UploadHomeTimeline_result&);
  FakeRabbitmq_UploadHomeTimeline_result() {
  }

  virtual ~FakeRabbitmq_UploadHomeTimeline_result() throw();
  ServiceException se;

  _FakeRabbitmq_UploadHomeTimeline_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeRabbitmq_UploadHomeTimeline_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeRabbitmq_UploadHomeTimeline_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeRabbitmq_UploadHomeTimeline_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeRabbitmq_UploadHomeTimeline_presult__isset {
  _FakeRabbitmq_UploadHomeTimeline_presult__isset() : se(false) {}
  bool se :1;
} _FakeRabbitmq_UploadHomeTimeline_presult__isset;

class FakeRabbitmq_UploadHomeTimeline_presult {
 public:


  virtual ~FakeRabbitmq_UploadHomeTimeline_presult() throw();
  ServiceException se;

  _FakeRabbitmq_UploadHomeTimeline_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class FakeRabbitmqProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeRabbitmqIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (FakeRabbitmqProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_UploadHomeTimeline(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  FakeRabbitmqProcessor(::apache::thrift::stdcxx::shared_ptr<FakeRabbitmqIf> iface) :
    iface_(iface) {
    processMap_["UploadHomeTimeline"] = &FakeRabbitmqProcessor::process_UploadHomeTimeline;
  }

  virtual ~FakeRabbitmqProcessor() {}
};

class FakeRabbitmqHandler : virtual public FakeRabbitmqIf {
  public:
    FakeRabbitmqHandler() = default;
    ~FakeRabbitmqHandler() = default;

    void UploadHomeTimeline(const int64_t req_id, const int64_t post_id, const int64_t user_id, const int64_t timestamp, const std::vector<int64_t> & user_mentions_id);
};

class FakeRabbitmqClient : virtual public FakeRabbitmqIf {
 public:
  FakeRabbitmqClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
    std::shared_ptr<FakeRabbitmqHandler> handler = std::make_shared<FakeRabbitmqHandler>();
    _fakeProcessor = std::make_shared<FakeRabbitmqProcessor>(handler);
  }
  FakeRabbitmqClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
    std::shared_ptr<FakeRabbitmqHandler> handler = std::make_shared<FakeRabbitmqHandler>();
    _fakeProcessor = std::make_shared<FakeRabbitmqProcessor>(handler);
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
  void UploadHomeTimeline(const int64_t req_id, const int64_t post_id, const int64_t user_id, const int64_t timestamp, const std::vector<int64_t> & user_mentions_id);
  void send_UploadHomeTimeline(const int64_t req_id, const int64_t post_id, const int64_t user_id, const int64_t timestamp, const std::vector<int64_t> & user_mentions_id);
  void recv_UploadHomeTimeline();
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;

 private:
  std::shared_ptr<FakeRabbitmqProcessor> _fakeProcessor;
 public:
  static bool isReqGenPhase;
};

class FakeRabbitmqProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  FakeRabbitmqProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< FakeRabbitmqIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< FakeRabbitmqIfFactory > handlerFactory_;
};

class FakeRabbitmqMultiface : virtual public FakeRabbitmqIf {
 public:
  FakeRabbitmqMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<FakeRabbitmqIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~FakeRabbitmqMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<FakeRabbitmqIf> > ifaces_;
  FakeRabbitmqMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<FakeRabbitmqIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void UploadHomeTimeline(const int64_t req_id, const int64_t post_id, const int64_t user_id, const int64_t timestamp, const std::vector<int64_t> & user_mentions_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadHomeTimeline(req_id, post_id, user_id, timestamp, user_mentions_id);
    }
    ifaces_[i]->UploadHomeTimeline(req_id, post_id, user_id, timestamp, user_mentions_id);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class FakeRabbitmqConcurrentClient : virtual public FakeRabbitmqIf {
 public:
  FakeRabbitmqConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FakeRabbitmqConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void UploadHomeTimeline(const int64_t req_id, const int64_t post_id, const int64_t user_id, const int64_t timestamp, const std::vector<int64_t> & user_mentions_id);
  int32_t send_UploadHomeTimeline(const int64_t req_id, const int64_t post_id, const int64_t user_id, const int64_t timestamp, const std::vector<int64_t> & user_mentions_id);
  void recv_UploadHomeTimeline(const int32_t seqid);
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
