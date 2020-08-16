/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef FakeComposePostService_H
#define FakeComposePostService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "my_social_network_types.h"

#include <thread>
#include "../MyCommon/readerwriterqueue.h"
#include "../MyCommon/core_schedule.h"
#include "../MyCommon/atomicops.h"
#include "../MyCommon/stopwatch.h"

#include "FakeComposePostService.fwd.h"
#include "../MyCommon/PostPSendStage.fwd.h"
#include "../MyCommon/PrePRecvStage.fwd.h"

#include <iostream>

using namespace moodycamel;

namespace my_social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class FakeComposePostServiceIf {
 public:
  virtual ~FakeComposePostServiceIf() {}
  virtual void UploadText(const int64_t req_id, const std::string& text) = 0;
  virtual void UploadMedia(const int64_t req_id, const std::vector<Media> & media) = 0;
  virtual void UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type) = 0;
  virtual void UploadCreator(const int64_t req_id, const Creator& creator) = 0;
  virtual void UploadUrls(const int64_t req_id, const std::vector<Url> & urls) = 0;
  virtual void UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions) = 0;
};

class FakeComposePostServiceIfFactory {
 public:
  typedef FakeComposePostServiceIf Handler;

  virtual ~FakeComposePostServiceIfFactory() {}

  virtual FakeComposePostServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(FakeComposePostServiceIf* /* handler */) = 0;
};

class FakeComposePostServiceIfSingletonFactory : virtual public FakeComposePostServiceIfFactory {
 public:
  FakeComposePostServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<FakeComposePostServiceIf>& iface) : iface_(iface) {}
  virtual ~FakeComposePostServiceIfSingletonFactory() {}

  virtual FakeComposePostServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(FakeComposePostServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeComposePostServiceIf> iface_;
};

class FakeComposePostServiceNull : virtual public FakeComposePostServiceIf {
 public:
  virtual ~FakeComposePostServiceNull() {}
  void UploadText(const int64_t /* req_id */, const std::string& /* text */) {
    return;
  }
  void UploadMedia(const int64_t /* req_id */, const std::vector<Media> & /* media */) {
    return;
  }
  void UploadUniqueId(const int64_t /* req_id */, const int64_t /* post_id */, const PostType::type /* post_type */) {
    return;
  }
  void UploadCreator(const int64_t /* req_id */, const Creator& /* creator */) {
    return;
  }
  void UploadUrls(const int64_t /* req_id */, const std::vector<Url> & /* urls */) {
    return;
  }
  void UploadUserMentions(const int64_t /* req_id */, const std::vector<UserMention> & /* user_mentions */) {
    return;
  }
};

typedef struct _FakeComposePostService_UploadText_args__isset {
  _FakeComposePostService_UploadText_args__isset() : req_id(false), text(false) {}
  bool req_id :1;
  bool text :1;
} _FakeComposePostService_UploadText_args__isset;

class FakeComposePostService_UploadText_args {
 public:

  FakeComposePostService_UploadText_args(const FakeComposePostService_UploadText_args&);
  FakeComposePostService_UploadText_args& operator=(const FakeComposePostService_UploadText_args&);
  FakeComposePostService_UploadText_args() : req_id(0), text() {
  }

  virtual ~FakeComposePostService_UploadText_args() throw();
  int64_t req_id;
  std::string text;

  _FakeComposePostService_UploadText_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_text(const std::string& val);

  bool operator == (const FakeComposePostService_UploadText_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(text == rhs.text))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadText_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadText_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeComposePostService_UploadText_pargs {
 public:


  virtual ~FakeComposePostService_UploadText_pargs() throw();
  const int64_t* req_id;
  const std::string* text;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadText_result__isset {
  _FakeComposePostService_UploadText_result__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadText_result__isset;

class FakeComposePostService_UploadText_result {
 public:

  FakeComposePostService_UploadText_result(const FakeComposePostService_UploadText_result&);
  FakeComposePostService_UploadText_result& operator=(const FakeComposePostService_UploadText_result&);
  FakeComposePostService_UploadText_result() {
  }

  virtual ~FakeComposePostService_UploadText_result() throw();
  ServiceException se;

  _FakeComposePostService_UploadText_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeComposePostService_UploadText_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadText_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadText_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadText_presult__isset {
  _FakeComposePostService_UploadText_presult__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadText_presult__isset;

class FakeComposePostService_UploadText_presult {
 public:


  virtual ~FakeComposePostService_UploadText_presult() throw();
  ServiceException se;

  _FakeComposePostService_UploadText_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeComposePostService_UploadMedia_args__isset {
  _FakeComposePostService_UploadMedia_args__isset() : req_id(false), media(false) {}
  bool req_id :1;
  bool media :1;
} _FakeComposePostService_UploadMedia_args__isset;

class FakeComposePostService_UploadMedia_args {
 public:

  FakeComposePostService_UploadMedia_args(const FakeComposePostService_UploadMedia_args&);
  FakeComposePostService_UploadMedia_args& operator=(const FakeComposePostService_UploadMedia_args&);
  FakeComposePostService_UploadMedia_args() : req_id(0) {
  }

  virtual ~FakeComposePostService_UploadMedia_args() throw();
  int64_t req_id;
  std::vector<Media>  media;

  _FakeComposePostService_UploadMedia_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_media(const std::vector<Media> & val);

  bool operator == (const FakeComposePostService_UploadMedia_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(media == rhs.media))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadMedia_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadMedia_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeComposePostService_UploadMedia_pargs {
 public:


  virtual ~FakeComposePostService_UploadMedia_pargs() throw();
  const int64_t* req_id;
  const std::vector<Media> * media;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadMedia_result__isset {
  _FakeComposePostService_UploadMedia_result__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadMedia_result__isset;

class FakeComposePostService_UploadMedia_result {
 public:

  FakeComposePostService_UploadMedia_result(const FakeComposePostService_UploadMedia_result&);
  FakeComposePostService_UploadMedia_result& operator=(const FakeComposePostService_UploadMedia_result&);
  FakeComposePostService_UploadMedia_result() {
  }

  virtual ~FakeComposePostService_UploadMedia_result() throw();
  ServiceException se;

  _FakeComposePostService_UploadMedia_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeComposePostService_UploadMedia_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadMedia_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadMedia_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadMedia_presult__isset {
  _FakeComposePostService_UploadMedia_presult__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadMedia_presult__isset;

class FakeComposePostService_UploadMedia_presult {
 public:


  virtual ~FakeComposePostService_UploadMedia_presult() throw();
  ServiceException se;

  _FakeComposePostService_UploadMedia_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeComposePostService_UploadUniqueId_args__isset {
  _FakeComposePostService_UploadUniqueId_args__isset() : req_id(false), post_id(false), post_type(false) {}
  bool req_id :1;
  bool post_id :1;
  bool post_type :1;
} _FakeComposePostService_UploadUniqueId_args__isset;

class FakeComposePostService_UploadUniqueId_args {
 public:

  FakeComposePostService_UploadUniqueId_args(const FakeComposePostService_UploadUniqueId_args&);
  FakeComposePostService_UploadUniqueId_args& operator=(const FakeComposePostService_UploadUniqueId_args&);
  FakeComposePostService_UploadUniqueId_args() : req_id(0), post_id(0), post_type((PostType::type)0) {
  }

  virtual ~FakeComposePostService_UploadUniqueId_args() throw();
  int64_t req_id;
  int64_t post_id;
  PostType::type post_type;

  _FakeComposePostService_UploadUniqueId_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post_id(const int64_t val);

  void __set_post_type(const PostType::type val);

  bool operator == (const FakeComposePostService_UploadUniqueId_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post_id == rhs.post_id))
      return false;
    if (!(post_type == rhs.post_type))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadUniqueId_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadUniqueId_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeComposePostService_UploadUniqueId_pargs {
 public:


  virtual ~FakeComposePostService_UploadUniqueId_pargs() throw();
  const int64_t* req_id;
  const int64_t* post_id;
  const PostType::type* post_type;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadUniqueId_result__isset {
  _FakeComposePostService_UploadUniqueId_result__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadUniqueId_result__isset;

class FakeComposePostService_UploadUniqueId_result {
 public:

  FakeComposePostService_UploadUniqueId_result(const FakeComposePostService_UploadUniqueId_result&);
  FakeComposePostService_UploadUniqueId_result& operator=(const FakeComposePostService_UploadUniqueId_result&);
  FakeComposePostService_UploadUniqueId_result() {
  }

  virtual ~FakeComposePostService_UploadUniqueId_result() throw();
  ServiceException se;

  _FakeComposePostService_UploadUniqueId_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeComposePostService_UploadUniqueId_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadUniqueId_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadUniqueId_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadUniqueId_presult__isset {
  _FakeComposePostService_UploadUniqueId_presult__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadUniqueId_presult__isset;

class FakeComposePostService_UploadUniqueId_presult {
 public:


  virtual ~FakeComposePostService_UploadUniqueId_presult() throw();
  ServiceException se;

  _FakeComposePostService_UploadUniqueId_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeComposePostService_UploadCreator_args__isset {
  _FakeComposePostService_UploadCreator_args__isset() : req_id(false), creator(false) {}
  bool req_id :1;
  bool creator :1;
} _FakeComposePostService_UploadCreator_args__isset;

class FakeComposePostService_UploadCreator_args {
 public:

  FakeComposePostService_UploadCreator_args(const FakeComposePostService_UploadCreator_args&);
  FakeComposePostService_UploadCreator_args& operator=(const FakeComposePostService_UploadCreator_args&);
  FakeComposePostService_UploadCreator_args() : req_id(0) {
  }

  virtual ~FakeComposePostService_UploadCreator_args() throw();
  int64_t req_id;
  Creator creator;

  _FakeComposePostService_UploadCreator_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_creator(const Creator& val);

  bool operator == (const FakeComposePostService_UploadCreator_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(creator == rhs.creator))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadCreator_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadCreator_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeComposePostService_UploadCreator_pargs {
 public:


  virtual ~FakeComposePostService_UploadCreator_pargs() throw();
  const int64_t* req_id;
  const Creator* creator;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadCreator_result__isset {
  _FakeComposePostService_UploadCreator_result__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadCreator_result__isset;

class FakeComposePostService_UploadCreator_result {
 public:

  FakeComposePostService_UploadCreator_result(const FakeComposePostService_UploadCreator_result&);
  FakeComposePostService_UploadCreator_result& operator=(const FakeComposePostService_UploadCreator_result&);
  FakeComposePostService_UploadCreator_result() {
  }

  virtual ~FakeComposePostService_UploadCreator_result() throw();
  ServiceException se;

  _FakeComposePostService_UploadCreator_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeComposePostService_UploadCreator_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadCreator_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadCreator_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadCreator_presult__isset {
  _FakeComposePostService_UploadCreator_presult__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadCreator_presult__isset;

class FakeComposePostService_UploadCreator_presult {
 public:


  virtual ~FakeComposePostService_UploadCreator_presult() throw();
  ServiceException se;

  _FakeComposePostService_UploadCreator_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeComposePostService_UploadUrls_args__isset {
  _FakeComposePostService_UploadUrls_args__isset() : req_id(false), urls(false) {}
  bool req_id :1;
  bool urls :1;
} _FakeComposePostService_UploadUrls_args__isset;

class FakeComposePostService_UploadUrls_args {
 public:

  FakeComposePostService_UploadUrls_args(const FakeComposePostService_UploadUrls_args&);
  FakeComposePostService_UploadUrls_args& operator=(const FakeComposePostService_UploadUrls_args&);
  FakeComposePostService_UploadUrls_args() : req_id(0) {
  }

  virtual ~FakeComposePostService_UploadUrls_args() throw();
  int64_t req_id;
  std::vector<Url>  urls;

  _FakeComposePostService_UploadUrls_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_urls(const std::vector<Url> & val);

  bool operator == (const FakeComposePostService_UploadUrls_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(urls == rhs.urls))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadUrls_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadUrls_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeComposePostService_UploadUrls_pargs {
 public:


  virtual ~FakeComposePostService_UploadUrls_pargs() throw();
  const int64_t* req_id;
  const std::vector<Url> * urls;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadUrls_result__isset {
  _FakeComposePostService_UploadUrls_result__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadUrls_result__isset;

class FakeComposePostService_UploadUrls_result {
 public:

  FakeComposePostService_UploadUrls_result(const FakeComposePostService_UploadUrls_result&);
  FakeComposePostService_UploadUrls_result& operator=(const FakeComposePostService_UploadUrls_result&);
  FakeComposePostService_UploadUrls_result() {
  }

  virtual ~FakeComposePostService_UploadUrls_result() throw();
  ServiceException se;

  _FakeComposePostService_UploadUrls_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeComposePostService_UploadUrls_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadUrls_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadUrls_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadUrls_presult__isset {
  _FakeComposePostService_UploadUrls_presult__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadUrls_presult__isset;

class FakeComposePostService_UploadUrls_presult {
 public:


  virtual ~FakeComposePostService_UploadUrls_presult() throw();
  ServiceException se;

  _FakeComposePostService_UploadUrls_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeComposePostService_UploadUserMentions_args__isset {
  _FakeComposePostService_UploadUserMentions_args__isset() : req_id(false), user_mentions(false) {}
  bool req_id :1;
  bool user_mentions :1;
} _FakeComposePostService_UploadUserMentions_args__isset;

class FakeComposePostService_UploadUserMentions_args {
 public:

  FakeComposePostService_UploadUserMentions_args(const FakeComposePostService_UploadUserMentions_args&);
  FakeComposePostService_UploadUserMentions_args& operator=(const FakeComposePostService_UploadUserMentions_args&);
  FakeComposePostService_UploadUserMentions_args() : req_id(0) {
  }

  virtual ~FakeComposePostService_UploadUserMentions_args() throw();
  int64_t req_id;
  std::vector<UserMention>  user_mentions;

  _FakeComposePostService_UploadUserMentions_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_user_mentions(const std::vector<UserMention> & val);

  bool operator == (const FakeComposePostService_UploadUserMentions_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(user_mentions == rhs.user_mentions))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadUserMentions_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadUserMentions_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeComposePostService_UploadUserMentions_pargs {
 public:


  virtual ~FakeComposePostService_UploadUserMentions_pargs() throw();
  const int64_t* req_id;
  const std::vector<UserMention> * user_mentions;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadUserMentions_result__isset {
  _FakeComposePostService_UploadUserMentions_result__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadUserMentions_result__isset;

class FakeComposePostService_UploadUserMentions_result {
 public:

  FakeComposePostService_UploadUserMentions_result(const FakeComposePostService_UploadUserMentions_result&);
  FakeComposePostService_UploadUserMentions_result& operator=(const FakeComposePostService_UploadUserMentions_result&);
  FakeComposePostService_UploadUserMentions_result() {
  }

  virtual ~FakeComposePostService_UploadUserMentions_result() throw();
  ServiceException se;

  _FakeComposePostService_UploadUserMentions_result__isset __isset;

  void __set_se(const ServiceException& val);

  bool operator == (const FakeComposePostService_UploadUserMentions_result & rhs) const
  {
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const FakeComposePostService_UploadUserMentions_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeComposePostService_UploadUserMentions_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeComposePostService_UploadUserMentions_presult__isset {
  _FakeComposePostService_UploadUserMentions_presult__isset() : se(false) {}
  bool se :1;
} _FakeComposePostService_UploadUserMentions_presult__isset;

class FakeComposePostService_UploadUserMentions_presult {
 public:


  virtual ~FakeComposePostService_UploadUserMentions_presult() throw();
  ServiceException se;

  _FakeComposePostService_UploadUserMentions_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class FakeComposePostServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeComposePostServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (FakeComposePostServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_UploadText(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadMedia(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadUniqueId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadCreator(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadUrls(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadUserMentions(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  FakeComposePostServiceProcessor(::apache::thrift::stdcxx::shared_ptr<FakeComposePostServiceIf> iface) :
    iface_(iface) {
    processMap_["UploadText"] = &FakeComposePostServiceProcessor::process_UploadText;
    processMap_["UploadMedia"] = &FakeComposePostServiceProcessor::process_UploadMedia;
    processMap_["UploadUniqueId"] = &FakeComposePostServiceProcessor::process_UploadUniqueId;
    processMap_["UploadCreator"] = &FakeComposePostServiceProcessor::process_UploadCreator;
    processMap_["UploadUrls"] = &FakeComposePostServiceProcessor::process_UploadUrls;
    processMap_["UploadUserMentions"] = &FakeComposePostServiceProcessor::process_UploadUserMentions;
  }

  virtual ~FakeComposePostServiceProcessor() {}
};

class FakeComposePostHandler : public FakeComposePostServiceIf {
 public:
  FakeComposePostHandler() = default;
  ~FakeComposePostHandler() = default;

  void UploadUniqueId(int64_t req_id, const int64_t post_id,
      const PostType::type post_type) override;


  void UploadText(int64_t req_id, const std::string& text) override;
  void UploadMedia(int64_t req_id, const std::vector<Media>& media) override;
  void UploadCreator(int64_t req_id, const Creator& creator) override;
  void UploadUrls(int64_t req_id, const std::vector<Url> & urls) override;
  void UploadUserMentions(const int64_t req_id,
      const std::vector<UserMention> & user_mentions) override;

 private:

};

class FakeComposePostServiceClient : virtual public FakeComposePostServiceIf {
 public:
  FakeComposePostServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }

  FakeComposePostServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot,
                               apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
    std::shared_ptr<FakeComposePostHandler> handler = std::make_shared<FakeComposePostHandler>();
    _fakeProcessor = std::make_shared<FakeComposePostServiceProcessor>(handler);

    processorThread = std::thread([this] {Run_();});
    int coreID = PinToCore(&processorThread, false);
    std::cout << "FakeProcessor pined to core " << coreID << std::endl;
  }

  FakeComposePostServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot,
                               apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot,
                               PostPSendStage* postpSendStageHandler,
                               PrePRecvStage* prepRecvStageHandler) {
    setProtocol(iprot,oprot);
    std::shared_ptr<FakeComposePostHandler> handler = std::make_shared<FakeComposePostHandler>();
    _fakeProcessor = std::make_shared<FakeComposePostServiceProcessor>(handler);

    processorThread = std::thread([this] {Run_();});
    PinToCore(&processorThread, false);

    #ifdef STAGED
    // recvThread_ = std::thread([this] {Recv();});
    // int coreId = PinToCore(&recvThread_);
    _postpSendStageHandler = postpSendStageHandler;
    _prepRecvStageHandler = prepRecvStageHandler;
    // std::cout << "Recv thread pinned to core " << coreId << "." << std::endl;
    #endif
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
  void UploadText(const int64_t req_id, const std::string& text);
  void send_UploadText(const int64_t req_id, const std::string& text);
  void recv_UploadText();
  void UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  void send_UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  void recv_UploadMedia();
  void UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  void send_UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  void recv_UploadUniqueId();
  void UploadCreator(const int64_t req_id, const Creator& creator);
  void send_UploadCreator(const int64_t req_id, const Creator& creator);
  void recv_UploadCreator();
  void UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  void send_UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  void recv_UploadUrls();
  void UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  void send_UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  void recv_UploadUserMentions();
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;

 private:
  std::shared_ptr<FakeComposePostServiceProcessor> _fakeProcessor;
  std::thread processorThread;
  ReaderWriterQueue<int> RQ_;
  BlockingReaderWriterQueue<int> CQ_;
  std::atomic<bool> exit_flag_{false};

  void Run_();


 public:

  #if defined(SW) && !defined(STAGED)
  Stopwatch<std::chrono::nanoseconds> recvSW_;
  Stopwatch<std::chrono::nanoseconds> sendSW_;
  #endif

  #ifdef STAGED
  PostPSendStage* _postpSendStageHandler;
  PrePRecvStage* _prepRecvStageHandler;
  #endif

  ~FakeComposePostServiceClient() {
    exit_flag_ = true;
    processorThread.join();

    #ifdef STAGED
    // exit_recvT_ = true;
    // recvThread_.join();
    #endif

    #if defined(SW) && !defined(STAGED)
    recvSW_.post_process();
    std::cout << "Recv(ns): " << recvSW_.mean() << std::endl;

    sendSW_.post_process();
    std::cout << "Send(ns): " << sendSW_.mean() << std::endl;
    #endif
  }

};

class FakeComposePostServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  FakeComposePostServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< FakeComposePostServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< FakeComposePostServiceIfFactory > handlerFactory_;
};

class FakeComposePostServiceMultiface : virtual public FakeComposePostServiceIf {
 public:
  FakeComposePostServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<FakeComposePostServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~FakeComposePostServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<FakeComposePostServiceIf> > ifaces_;
  FakeComposePostServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<FakeComposePostServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void UploadText(const int64_t req_id, const std::string& text) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadText(req_id, text);
    }
    ifaces_[i]->UploadText(req_id, text);
  }

  void UploadMedia(const int64_t req_id, const std::vector<Media> & media) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadMedia(req_id, media);
    }
    ifaces_[i]->UploadMedia(req_id, media);
  }

  void UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUniqueId(req_id, post_id, post_type);
    }
    ifaces_[i]->UploadUniqueId(req_id, post_id, post_type);
  }

  void UploadCreator(const int64_t req_id, const Creator& creator) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadCreator(req_id, creator);
    }
    ifaces_[i]->UploadCreator(req_id, creator);
  }

  void UploadUrls(const int64_t req_id, const std::vector<Url> & urls) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUrls(req_id, urls);
    }
    ifaces_[i]->UploadUrls(req_id, urls);
  }

  void UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUserMentions(req_id, user_mentions);
    }
    ifaces_[i]->UploadUserMentions(req_id, user_mentions);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class FakeComposePostServiceConcurrentClient : virtual public FakeComposePostServiceIf {
 public:
  FakeComposePostServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FakeComposePostServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void UploadText(const int64_t req_id, const std::string& text);
  int32_t send_UploadText(const int64_t req_id, const std::string& text);
  void recv_UploadText(const int32_t seqid);
  void UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  int32_t send_UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  void recv_UploadMedia(const int32_t seqid);
  void UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  int32_t send_UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  void recv_UploadUniqueId(const int32_t seqid);
  void UploadCreator(const int64_t req_id, const Creator& creator);
  int32_t send_UploadCreator(const int64_t req_id, const Creator& creator);
  void recv_UploadCreator(const int32_t seqid);
  void UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  int32_t send_UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  void recv_UploadUrls(const int32_t seqid);
  void UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  int32_t send_UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  void recv_UploadUserMentions(const int32_t seqid);
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
