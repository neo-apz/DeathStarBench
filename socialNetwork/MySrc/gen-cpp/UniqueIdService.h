/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef UniqueIdService_H
#define UniqueIdService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "my_social_network_types.h"

#ifdef __aarch64__
	#include "MagicBreakPoint.h"
#endif

namespace my_social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class UniqueIdServiceIf {
 public:
  virtual ~UniqueIdServiceIf() {}
  virtual int64_t UploadUniqueId(const int64_t req_id, const PostType::type post_type) = 0;

	struct FuncType {
  enum type {
    UPLOAD_UNIQUE_ID = 0,
    
		SIZE = 1
  	};
	};
};

class UniqueIdServiceIfFactory {
 public:
  typedef UniqueIdServiceIf Handler;

  virtual ~UniqueIdServiceIfFactory() {}

  virtual UniqueIdServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(UniqueIdServiceIf* /* handler */) = 0;
};

class UniqueIdServiceIfSingletonFactory : virtual public UniqueIdServiceIfFactory {
 public:
  UniqueIdServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf>& iface) : iface_(iface) {}
  virtual ~UniqueIdServiceIfSingletonFactory() {}

  virtual UniqueIdServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(UniqueIdServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> iface_;
};

class UniqueIdServiceNull : virtual public UniqueIdServiceIf {
 public:
  virtual ~UniqueIdServiceNull() {}
  int64_t UploadUniqueId(const int64_t /* req_id */, const PostType::type /* post_type */) {
    int64_t _return = 0;
    return _return;
  }
};

typedef struct _UniqueIdService_UploadUniqueId_args__isset {
  _UniqueIdService_UploadUniqueId_args__isset() : req_id(false), post_type(false) {}
  bool req_id :1;
  bool post_type :1;
} _UniqueIdService_UploadUniqueId_args__isset;

class UniqueIdService_UploadUniqueId_args {
 public:

  UniqueIdService_UploadUniqueId_args(const UniqueIdService_UploadUniqueId_args&);
  UniqueIdService_UploadUniqueId_args& operator=(const UniqueIdService_UploadUniqueId_args&);
  UniqueIdService_UploadUniqueId_args() : req_id(0), post_type((PostType::type)0) {
  }

	UniqueIdService_UploadUniqueId_args(RandomGenerator *randGen) {
		req_id = randGen->getInt64(RAND_NUM_LIMIT);
	  post_type = (PostType::type) randGen->getInt64(0, 3);
  }

  virtual ~UniqueIdService_UploadUniqueId_args() throw();
  int64_t req_id;
  PostType::type post_type;

  _UniqueIdService_UploadUniqueId_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post_type(const PostType::type val);

  bool operator == (const UniqueIdService_UploadUniqueId_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post_type == rhs.post_type))
      return false;
    return true;
  }
  bool operator != (const UniqueIdService_UploadUniqueId_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UniqueIdService_UploadUniqueId_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class UniqueIdService_UploadUniqueId_pargs {
 public:


  virtual ~UniqueIdService_UploadUniqueId_pargs() throw();
  const int64_t* req_id;
  const PostType::type* post_type;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _UniqueIdService_UploadUniqueId_result__isset {
  _UniqueIdService_UploadUniqueId_result__isset() : success(false) {}
  bool success :1;
} _UniqueIdService_UploadUniqueId_result__isset;

class UniqueIdService_UploadUniqueId_result {
 public:

  UniqueIdService_UploadUniqueId_result(const UniqueIdService_UploadUniqueId_result&);
  UniqueIdService_UploadUniqueId_result& operator=(const UniqueIdService_UploadUniqueId_result&);
  UniqueIdService_UploadUniqueId_result() : success(0) {
  }

  virtual ~UniqueIdService_UploadUniqueId_result() throw();
  int64_t success;

  _UniqueIdService_UploadUniqueId_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const UniqueIdService_UploadUniqueId_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const UniqueIdService_UploadUniqueId_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UniqueIdService_UploadUniqueId_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _UniqueIdService_UploadUniqueId_presult__isset {
  _UniqueIdService_UploadUniqueId_presult__isset() : success(false) {}
  bool success :1;
} _UniqueIdService_UploadUniqueId_presult__isset;

class UniqueIdService_UploadUniqueId_presult {
 public:


  virtual ~UniqueIdService_UploadUniqueId_presult() throw();
  int64_t* success;

  _UniqueIdService_UploadUniqueId_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class UniqueIdServiceClient : virtual public UniqueIdServiceIf {
 public:
  UniqueIdServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  UniqueIdServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
	UniqueIdServiceClient(RandomGenerator* randGen) {
		initArgs(randGen);
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
  int64_t UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  void send_UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  int64_t recv_UploadUniqueId();

	UniqueIdService_UploadUniqueId_args *uploadUniqueId_args;
	void initArgs(RandomGenerator* randGen);
	void send_RandReq(RandomGenerator* randGen);

	UniqueIdService_UploadUniqueId_result uploadUniqueId_res;
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class UniqueIdServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> iface_;
 private:
  typedef  void (UniqueIdServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_UploadUniqueId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  UniqueIdServiceProcessor(::apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> iface) :
    iface_(iface) {
    processMap_["UploadUniqueId"] = &UniqueIdServiceProcessor::process_UploadUniqueId;
  }
	virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);

  virtual ~UniqueIdServiceProcessor() {}
};

class UniqueIdServiceCerebrosProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> iface_;
 private:
  typedef  uint64_t (UniqueIdServiceCerebrosProcessor::*ProcessFunction)(UniqueIdServiceClient*);
  // typedef struct ProcessMap { ProcessFunction x[UniqueIdServiceIf::FuncType::SIZE]; } ProcessMap;
	typedef ProcessFunction ProcessMap[UniqueIdServiceIf::FuncType::SIZE];
  ProcessMap processMap_;
  uint64_t process_UploadUniqueId(UniqueIdServiceClient* client);
 public:
  UniqueIdServiceCerebrosProcessor(::apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> iface) :
    iface_(iface) {
    processMap_[UniqueIdServiceIf::FuncType::UPLOAD_UNIQUE_ID] = &UniqueIdServiceCerebrosProcessor::process_UploadUniqueId;
  }
	uint64_t dispatchCall(int funcCode, UniqueIdServiceClient* client) {
		if (funcCode >= UniqueIdServiceIf::FuncType::SIZE){
			printf("Bad func code (%d) to dispatch in UniqueIdService!\n", funcCode);
			exit(1);
		}

		return (this->*(processMap_[funcCode]))(client);
	}

  virtual ~UniqueIdServiceCerebrosProcessor() {}
};

class UniqueIdServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  UniqueIdServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< UniqueIdServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< UniqueIdServiceIfFactory > handlerFactory_;
};

class UniqueIdServiceMultiface : virtual public UniqueIdServiceIf {
 public:
  UniqueIdServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~UniqueIdServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> > ifaces_;
  UniqueIdServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<UniqueIdServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int64_t UploadUniqueId(const int64_t req_id, const PostType::type post_type) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUniqueId(req_id, post_type);
    }
    return ifaces_[i]->UploadUniqueId(req_id, post_type);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class UniqueIdServiceConcurrentClient : virtual public UniqueIdServiceIf {
 public:
  UniqueIdServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  UniqueIdServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  int64_t UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  int32_t send_UploadUniqueId(const int64_t req_id, const PostType::type post_type);
  int64_t recv_UploadUniqueId(const int32_t seqid);
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
