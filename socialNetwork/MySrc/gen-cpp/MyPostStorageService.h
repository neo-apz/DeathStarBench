/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef MyPostStorageService_H
#define MyPostStorageService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "my_social_network_types.h"

namespace my_social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class MyPostStorageServiceIf {
 public:
  virtual ~MyPostStorageServiceIf() {}
  virtual void StorePost(const int64_t req_id, const Post& post) = 0;
  virtual void ReadPost(Post& _return, const int64_t req_id, const int64_t post_id) = 0;
  virtual void ReadPosts(std::vector<Post> & _return, const int64_t req_id, const std::vector<int64_t> & post_ids) = 0;
};

class MyPostStorageServiceIfFactory {
 public:
  typedef MyPostStorageServiceIf Handler;

  virtual ~MyPostStorageServiceIfFactory() {}

  virtual MyPostStorageServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(MyPostStorageServiceIf* /* handler */) = 0;
};

class MyPostStorageServiceIfSingletonFactory : virtual public MyPostStorageServiceIfFactory {
 public:
  MyPostStorageServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<MyPostStorageServiceIf>& iface) : iface_(iface) {}
  virtual ~MyPostStorageServiceIfSingletonFactory() {}

  virtual MyPostStorageServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(MyPostStorageServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<MyPostStorageServiceIf> iface_;
};

class MyPostStorageServiceNull : virtual public MyPostStorageServiceIf {
 public:
  virtual ~MyPostStorageServiceNull() {}
  void StorePost(const int64_t /* req_id */, const Post& /* post */) {
    return;
  }
  void ReadPost(Post& /* _return */, const int64_t /* req_id */, const int64_t /* post_id */) {
    return;
  }
  void ReadPosts(std::vector<Post> & /* _return */, const int64_t /* req_id */, const std::vector<int64_t> & /* post_ids */) {
    return;
  }
};

typedef struct _MyPostStorageService_StorePost_args__isset {
  _MyPostStorageService_StorePost_args__isset() : req_id(false), post(false) {}
  bool req_id :1;
  bool post :1;
} _MyPostStorageService_StorePost_args__isset;

class MyPostStorageService_StorePost_args {
 public:

  MyPostStorageService_StorePost_args(const MyPostStorageService_StorePost_args&);
  MyPostStorageService_StorePost_args& operator=(const MyPostStorageService_StorePost_args&);
  MyPostStorageService_StorePost_args() : req_id(0) {
  }

  virtual ~MyPostStorageService_StorePost_args() throw();
  int64_t req_id;
  Post post;

  _MyPostStorageService_StorePost_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post(const Post& val);

  bool operator == (const MyPostStorageService_StorePost_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post == rhs.post))
      return false;
    return true;
  }
  bool operator != (const MyPostStorageService_StorePost_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyPostStorageService_StorePost_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MyPostStorageService_StorePost_pargs {
 public:


  virtual ~MyPostStorageService_StorePost_pargs() throw();
  const int64_t* req_id;
  const Post* post;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyPostStorageService_StorePost_result__isset {
  _MyPostStorageService_StorePost_result__isset() : se(false) {}
  bool se :1;
} _MyPostStorageService_StorePost_result__isset;

class MyPostStorageService_StorePost_result {
 public:

  MyPostStorageService_StorePost_result(const MyPostStorageService_StorePost_result&);
  MyPostStorageService_StorePost_result& operator=(const MyPostStorageService_StorePost_result&);
  MyPostStorageService_StorePost_result() {
  }

  virtual ~MyPostStorageService_StorePost_result() throw();
  ServiceException se;

  _MyPostStorageService_StorePost_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const MyPostStorageService_StorePost_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const MyPostStorageService_StorePost_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyPostStorageService_StorePost_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyPostStorageService_StorePost_presult__isset {
  _MyPostStorageService_StorePost_presult__isset() : se(false) {}
  bool se :1;
} _MyPostStorageService_StorePost_presult__isset;

class MyPostStorageService_StorePost_presult {
 public:


  virtual ~MyPostStorageService_StorePost_presult() throw();
  ServiceException se;

  _MyPostStorageService_StorePost_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _MyPostStorageService_ReadPost_args__isset {
  _MyPostStorageService_ReadPost_args__isset() : req_id(false), post_id(false) {}
  bool req_id :1;
  bool post_id :1;
} _MyPostStorageService_ReadPost_args__isset;

class MyPostStorageService_ReadPost_args {
 public:

  MyPostStorageService_ReadPost_args(const MyPostStorageService_ReadPost_args&);
  MyPostStorageService_ReadPost_args& operator=(const MyPostStorageService_ReadPost_args&);
  MyPostStorageService_ReadPost_args() : req_id(0), post_id(0) {
  }

  virtual ~MyPostStorageService_ReadPost_args() throw();
  int64_t req_id;
  int64_t post_id;

  _MyPostStorageService_ReadPost_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post_id(const int64_t val);

  bool operator == (const MyPostStorageService_ReadPost_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post_id == rhs.post_id))
      return false;
    return true;
  }
  bool operator != (const MyPostStorageService_ReadPost_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyPostStorageService_ReadPost_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MyPostStorageService_ReadPost_pargs {
 public:


  virtual ~MyPostStorageService_ReadPost_pargs() throw();
  const int64_t* req_id;
  const int64_t* post_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyPostStorageService_ReadPost_result__isset {
  _MyPostStorageService_ReadPost_result__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MyPostStorageService_ReadPost_result__isset;

class MyPostStorageService_ReadPost_result {
 public:

  MyPostStorageService_ReadPost_result(const MyPostStorageService_ReadPost_result&);
  MyPostStorageService_ReadPost_result& operator=(const MyPostStorageService_ReadPost_result&);
  MyPostStorageService_ReadPost_result() {
  }

  virtual ~MyPostStorageService_ReadPost_result() throw();
  Post success;
  ServiceException se;

  _MyPostStorageService_ReadPost_result__isset __isset;

  void __set_success(const Post& val);

  void __set_se(const ServiceException& val);

  bool operator == (const MyPostStorageService_ReadPost_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const MyPostStorageService_ReadPost_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyPostStorageService_ReadPost_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyPostStorageService_ReadPost_presult__isset {
  _MyPostStorageService_ReadPost_presult__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MyPostStorageService_ReadPost_presult__isset;

class MyPostStorageService_ReadPost_presult {
 public:


  virtual ~MyPostStorageService_ReadPost_presult() throw();
  Post* success;
  ServiceException se;

  _MyPostStorageService_ReadPost_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _MyPostStorageService_ReadPosts_args__isset {
  _MyPostStorageService_ReadPosts_args__isset() : req_id(false), post_ids(false) {}
  bool req_id :1;
  bool post_ids :1;
} _MyPostStorageService_ReadPosts_args__isset;

class MyPostStorageService_ReadPosts_args {
 public:

  MyPostStorageService_ReadPosts_args(const MyPostStorageService_ReadPosts_args&);
  MyPostStorageService_ReadPosts_args& operator=(const MyPostStorageService_ReadPosts_args&);
  MyPostStorageService_ReadPosts_args() : req_id(0) {
  }

  virtual ~MyPostStorageService_ReadPosts_args() throw();
  int64_t req_id;
  std::vector<int64_t>  post_ids;

  _MyPostStorageService_ReadPosts_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post_ids(const std::vector<int64_t> & val);

  bool operator == (const MyPostStorageService_ReadPosts_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post_ids == rhs.post_ids))
      return false;
    return true;
  }
  bool operator != (const MyPostStorageService_ReadPosts_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyPostStorageService_ReadPosts_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MyPostStorageService_ReadPosts_pargs {
 public:


  virtual ~MyPostStorageService_ReadPosts_pargs() throw();
  const int64_t* req_id;
  const std::vector<int64_t> * post_ids;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyPostStorageService_ReadPosts_result__isset {
  _MyPostStorageService_ReadPosts_result__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MyPostStorageService_ReadPosts_result__isset;

class MyPostStorageService_ReadPosts_result {
 public:

  MyPostStorageService_ReadPosts_result(const MyPostStorageService_ReadPosts_result&);
  MyPostStorageService_ReadPosts_result& operator=(const MyPostStorageService_ReadPosts_result&);
  MyPostStorageService_ReadPosts_result() {
  }

  virtual ~MyPostStorageService_ReadPosts_result() throw();
  std::vector<Post>  success;
  ServiceException se;

  _MyPostStorageService_ReadPosts_result__isset __isset;

  void __set_success(const std::vector<Post> & val);

  void __set_se(const ServiceException& val);

  bool operator == (const MyPostStorageService_ReadPosts_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const MyPostStorageService_ReadPosts_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MyPostStorageService_ReadPosts_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MyPostStorageService_ReadPosts_presult__isset {
  _MyPostStorageService_ReadPosts_presult__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MyPostStorageService_ReadPosts_presult__isset;

class MyPostStorageService_ReadPosts_presult {
 public:


  virtual ~MyPostStorageService_ReadPosts_presult() throw();
  std::vector<Post> * success;
  ServiceException se;

  _MyPostStorageService_ReadPosts_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MyPostStorageServiceClient : virtual public MyPostStorageServiceIf {
 public:
  MyPostStorageServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MyPostStorageServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void StorePost(const int64_t req_id, const Post& post);
  void send_StorePost(const int64_t req_id, const Post& post);
  void recv_StorePost();
  void ReadPost(Post& _return, const int64_t req_id, const int64_t post_id);
  void send_ReadPost(const int64_t req_id, const int64_t post_id);
  void recv_ReadPost(Post& _return);
  void ReadPosts(std::vector<Post> & _return, const int64_t req_id, const std::vector<int64_t> & post_ids);
  void send_ReadPosts(const int64_t req_id, const std::vector<int64_t> & post_ids);
  void recv_ReadPosts(std::vector<Post> & _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class MyPostStorageServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<MyPostStorageServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MyPostStorageServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_StorePost(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ReadPost(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ReadPosts(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  MyPostStorageServiceProcessor(::apache::thrift::stdcxx::shared_ptr<MyPostStorageServiceIf> iface) :
    iface_(iface) {
    processMap_["StorePost"] = &MyPostStorageServiceProcessor::process_StorePost;
    processMap_["ReadPost"] = &MyPostStorageServiceProcessor::process_ReadPost;
    processMap_["ReadPosts"] = &MyPostStorageServiceProcessor::process_ReadPosts;
  }

  virtual ~MyPostStorageServiceProcessor() {}
};

class MyPostStorageServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  MyPostStorageServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< MyPostStorageServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< MyPostStorageServiceIfFactory > handlerFactory_;
};

class MyPostStorageServiceMultiface : virtual public MyPostStorageServiceIf {
 public:
  MyPostStorageServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<MyPostStorageServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MyPostStorageServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<MyPostStorageServiceIf> > ifaces_;
  MyPostStorageServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<MyPostStorageServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void StorePost(const int64_t req_id, const Post& post) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->StorePost(req_id, post);
    }
    ifaces_[i]->StorePost(req_id, post);
  }

  void ReadPost(Post& _return, const int64_t req_id, const int64_t post_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ReadPost(_return, req_id, post_id);
    }
    ifaces_[i]->ReadPost(_return, req_id, post_id);
    return;
  }

  void ReadPosts(std::vector<Post> & _return, const int64_t req_id, const std::vector<int64_t> & post_ids) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ReadPosts(_return, req_id, post_ids);
    }
    ifaces_[i]->ReadPosts(_return, req_id, post_ids);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class MyPostStorageServiceConcurrentClient : virtual public MyPostStorageServiceIf {
 public:
  MyPostStorageServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MyPostStorageServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void StorePost(const int64_t req_id, const Post& post);
  int32_t send_StorePost(const int64_t req_id, const Post& post);
  void recv_StorePost(const int32_t seqid);
  void ReadPost(Post& _return, const int64_t req_id, const int64_t post_id);
  int32_t send_ReadPost(const int64_t req_id, const int64_t post_id);
  void recv_ReadPost(Post& _return, const int32_t seqid);
  void ReadPosts(std::vector<Post> & _return, const int64_t req_id, const std::vector<int64_t> & post_ids);
  int32_t send_ReadPosts(const int64_t req_id, const std::vector<int64_t> & post_ids);
  void recv_ReadPosts(std::vector<Post> & _return, const int32_t seqid);
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
