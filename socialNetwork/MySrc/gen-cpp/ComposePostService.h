/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ComposePostService_H
#define ComposePostService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "my_social_network_types.h"

#include <RandomGenerator.h>
#include <FunctionClientMap.h>

#include <iostream>

#ifdef __aarch64__
	#include "MagicBreakPoint.h"
#endif
namespace my_social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ComposePostServiceIf {
 public:
  virtual ~ComposePostServiceIf() {}
  virtual int64_t UploadText(const int64_t req_id, const std::string& text) = 0;
  virtual int64_t UploadMedia(const int64_t req_id, const std::vector<Media> & media) = 0;
  virtual int64_t UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type) = 0;
  virtual int64_t UploadCreator(const int64_t req_id, const Creator& creator) = 0;
  virtual int64_t UploadUrls(const int64_t req_id, const std::vector<Url> & urls) = 0;
  virtual int64_t UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions) = 0;

	struct FuncType {
  enum type {
    UPLOAD_TEXT = 0,
    UPLOAD_MEDIA = 1,
    UPLOAD_UNIQUE_ID = 2,
    UPLOAD_CREATOR = 3,
    UPLOAD_URLS = 4,
		UPLOAD_USER_MENTIONS = 5,
    
    SIZE = 6
  	};
	};
};

class ComposePostServiceIfFactory {
 public:
  typedef ComposePostServiceIf Handler;

  virtual ~ComposePostServiceIfFactory() {}

  virtual ComposePostServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ComposePostServiceIf* /* handler */) = 0;
};

class ComposePostServiceIfSingletonFactory : virtual public ComposePostServiceIfFactory {
 public:
  ComposePostServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf>& iface) : iface_(iface) {}
  virtual ~ComposePostServiceIfSingletonFactory() {}

  virtual ComposePostServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ComposePostServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> iface_;
};

class ComposePostServiceNull : virtual public ComposePostServiceIf {
 public:
  virtual ~ComposePostServiceNull() {}
  int64_t UploadText(const int64_t /* req_id */, const std::string& /* text */) {
    int64_t _return = 0;
    return _return;
  }
  int64_t UploadMedia(const int64_t /* req_id */, const std::vector<Media> & /* media */) {
    int64_t _return = 0;
    return _return;
  }
  int64_t UploadUniqueId(const int64_t /* req_id */, const int64_t /* post_id */, const PostType::type /* post_type */) {
    int64_t _return = 0;
    return _return;
  }
  int64_t UploadCreator(const int64_t /* req_id */, const Creator& /* creator */) {
    int64_t _return = 0;
    return _return;
  }
  int64_t UploadUrls(const int64_t /* req_id */, const std::vector<Url> & /* urls */) {
    int64_t _return = 0;
    return _return;
  }
  int64_t UploadUserMentions(const int64_t /* req_id */, const std::vector<UserMention> & /* user_mentions */) {
    int64_t _return = 0;
    return _return;
  }
};

typedef struct _ComposePostService_UploadText_args__isset {
  _ComposePostService_UploadText_args__isset() : req_id(false), text(false) {}
  bool req_id :1;
  bool text :1;
} _ComposePostService_UploadText_args__isset;

class ComposePostService_UploadText_args {
 public:

  ComposePostService_UploadText_args(const ComposePostService_UploadText_args&);
  ComposePostService_UploadText_args& operator=(const ComposePostService_UploadText_args&);
  ComposePostService_UploadText_args() : req_id(0), text() {
  }

	ComposePostService_UploadText_args(RandomGenerator* randGen) {
		req_id = randGen->getInt64(RAND_NUM_LIMIT);
		text = randGen->getRandText();
  }

  virtual ~ComposePostService_UploadText_args() throw();
  int64_t req_id;
  std::string text;

  _ComposePostService_UploadText_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_text(const std::string& val);

  bool operator == (const ComposePostService_UploadText_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(text == rhs.text))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadText_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadText_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposePostService_UploadText_pargs {
 public:


  virtual ~ComposePostService_UploadText_pargs() throw();
  const int64_t* req_id;
  const std::string* text;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadText_result__isset {
  _ComposePostService_UploadText_result__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadText_result__isset;

class ComposePostService_UploadText_result {
 public:

  ComposePostService_UploadText_result(const ComposePostService_UploadText_result&);
  ComposePostService_UploadText_result& operator=(const ComposePostService_UploadText_result&);
  ComposePostService_UploadText_result() : success(0) {
  }

	ComposePostService_UploadText_result(RandomGenerator *randGen) {
		success = randGen->getInt64(RAND_NUM_LIMIT);
		__isset.success = true;
	}

  virtual ~ComposePostService_UploadText_result() throw();
  int64_t success;

  _ComposePostService_UploadText_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const ComposePostService_UploadText_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadText_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadText_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadText_presult__isset {
  _ComposePostService_UploadText_presult__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadText_presult__isset;

class ComposePostService_UploadText_presult {
 public:


  virtual ~ComposePostService_UploadText_presult() throw();
  int64_t* success;

  _ComposePostService_UploadText_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ComposePostService_UploadMedia_args__isset {
  _ComposePostService_UploadMedia_args__isset() : req_id(false), media(false) {}
  bool req_id :1;
  bool media :1;
} _ComposePostService_UploadMedia_args__isset;

class ComposePostService_UploadMedia_args {
 public:

  ComposePostService_UploadMedia_args(const ComposePostService_UploadMedia_args&);
  ComposePostService_UploadMedia_args& operator=(const ComposePostService_UploadMedia_args&);
  ComposePostService_UploadMedia_args() : req_id(0) {
  }

	ComposePostService_UploadMedia_args(RandomGenerator* randGen) {
		req_id = randGen->getInt64(RAND_NUM_LIMIT);
		uint32_t iters = randGen->getUInt32(1, 2);
		for(int i=0; i < iters; i++){
			Media m(randGen);
			media.emplace_back(m);
		}
  }

  virtual ~ComposePostService_UploadMedia_args() throw();
  int64_t req_id;
  std::vector<Media>  media;

  _ComposePostService_UploadMedia_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_media(const std::vector<Media> & val);

  bool operator == (const ComposePostService_UploadMedia_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(media == rhs.media))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadMedia_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadMedia_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposePostService_UploadMedia_pargs {
 public:


  virtual ~ComposePostService_UploadMedia_pargs() throw();
  const int64_t* req_id;
  const std::vector<Media> * media;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadMedia_result__isset {
  _ComposePostService_UploadMedia_result__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadMedia_result__isset;

class ComposePostService_UploadMedia_result {
 public:

  ComposePostService_UploadMedia_result(const ComposePostService_UploadMedia_result&);
  ComposePostService_UploadMedia_result& operator=(const ComposePostService_UploadMedia_result&);
  ComposePostService_UploadMedia_result() : success(0) {
  }

	ComposePostService_UploadMedia_result(RandomGenerator *randGen) {
		success = randGen->getInt64(RAND_NUM_LIMIT);
		__isset.success = true;
	}

  virtual ~ComposePostService_UploadMedia_result() throw();
  int64_t success;

  _ComposePostService_UploadMedia_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const ComposePostService_UploadMedia_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadMedia_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadMedia_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadMedia_presult__isset {
  _ComposePostService_UploadMedia_presult__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadMedia_presult__isset;

class ComposePostService_UploadMedia_presult {
 public:


  virtual ~ComposePostService_UploadMedia_presult() throw();
  int64_t* success;

  _ComposePostService_UploadMedia_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ComposePostService_UploadUniqueId_args__isset {
  _ComposePostService_UploadUniqueId_args__isset() : req_id(false), post_id(false), post_type(false) {}
  bool req_id :1;
  bool post_id :1;
  bool post_type :1;
} _ComposePostService_UploadUniqueId_args__isset;

class ComposePostService_UploadUniqueId_args {
 public:

  ComposePostService_UploadUniqueId_args(const ComposePostService_UploadUniqueId_args&);
  ComposePostService_UploadUniqueId_args& operator=(const ComposePostService_UploadUniqueId_args&);
  ComposePostService_UploadUniqueId_args() : req_id(0), post_id(0), post_type((PostType::type)0) {
  }

	ComposePostService_UploadUniqueId_args(RandomGenerator* randGen) {
		req_id = randGen->getInt64(RAND_NUM_LIMIT);
		post_id = randGen->getInt64(RAND_NUM_LIMIT);
		PostType::type post_type = (PostType::type) randGen->getInt64(0, 3);
  }

  virtual ~ComposePostService_UploadUniqueId_args() throw();
  int64_t req_id;
  int64_t post_id;
  PostType::type post_type;

  _ComposePostService_UploadUniqueId_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_post_id(const int64_t val);

  void __set_post_type(const PostType::type val);

  bool operator == (const ComposePostService_UploadUniqueId_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(post_id == rhs.post_id))
      return false;
    if (!(post_type == rhs.post_type))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadUniqueId_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadUniqueId_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposePostService_UploadUniqueId_pargs {
 public:


  virtual ~ComposePostService_UploadUniqueId_pargs() throw();
  const int64_t* req_id;
  const int64_t* post_id;
  const PostType::type* post_type;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadUniqueId_result__isset {
  _ComposePostService_UploadUniqueId_result__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadUniqueId_result__isset;

class ComposePostService_UploadUniqueId_result {
 public:

  ComposePostService_UploadUniqueId_result(const ComposePostService_UploadUniqueId_result&);
  ComposePostService_UploadUniqueId_result& operator=(const ComposePostService_UploadUniqueId_result&);
  ComposePostService_UploadUniqueId_result() : success(0) {
  }

	ComposePostService_UploadUniqueId_result(RandomGenerator *randGen) {
		success = randGen->getInt64(RAND_NUM_LIMIT);
		__isset.success = true;
	}

  virtual ~ComposePostService_UploadUniqueId_result() throw();
  int64_t success;

  _ComposePostService_UploadUniqueId_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const ComposePostService_UploadUniqueId_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadUniqueId_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadUniqueId_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadUniqueId_presult__isset {
  _ComposePostService_UploadUniqueId_presult__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadUniqueId_presult__isset;

class ComposePostService_UploadUniqueId_presult {
 public:


  virtual ~ComposePostService_UploadUniqueId_presult() throw();
  int64_t* success;

  _ComposePostService_UploadUniqueId_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ComposePostService_UploadCreator_args__isset {
  _ComposePostService_UploadCreator_args__isset() : req_id(false), creator(false) {}
  bool req_id :1;
  bool creator :1;
} _ComposePostService_UploadCreator_args__isset;

class ComposePostService_UploadCreator_args {
 public:

  ComposePostService_UploadCreator_args(const ComposePostService_UploadCreator_args&);
  ComposePostService_UploadCreator_args& operator=(const ComposePostService_UploadCreator_args&);
  ComposePostService_UploadCreator_args() : req_id(0) {
  }

	ComposePostService_UploadCreator_args(RandomGenerator* randGen) {
		req_id = randGen->getInt64(RAND_NUM_LIMIT);
		creator = Creator(randGen);
  }

  virtual ~ComposePostService_UploadCreator_args() throw();
  int64_t req_id;
  Creator creator;

  _ComposePostService_UploadCreator_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_creator(const Creator& val);

  bool operator == (const ComposePostService_UploadCreator_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(creator == rhs.creator))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadCreator_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadCreator_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposePostService_UploadCreator_pargs {
 public:


  virtual ~ComposePostService_UploadCreator_pargs() throw();
  const int64_t* req_id;
  const Creator* creator;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadCreator_result__isset {
  _ComposePostService_UploadCreator_result__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadCreator_result__isset;

class ComposePostService_UploadCreator_result {
 public:

  ComposePostService_UploadCreator_result(const ComposePostService_UploadCreator_result&);
  ComposePostService_UploadCreator_result& operator=(const ComposePostService_UploadCreator_result&);
  ComposePostService_UploadCreator_result() : success(0) {
  }

	ComposePostService_UploadCreator_result(RandomGenerator *randGen) {
		success = randGen->getInt64(RAND_NUM_LIMIT);
		__isset.success = true;
	}

  virtual ~ComposePostService_UploadCreator_result() throw();
  int64_t success;

  _ComposePostService_UploadCreator_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const ComposePostService_UploadCreator_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadCreator_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadCreator_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadCreator_presult__isset {
  _ComposePostService_UploadCreator_presult__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadCreator_presult__isset;

class ComposePostService_UploadCreator_presult {
 public:


  virtual ~ComposePostService_UploadCreator_presult() throw();
  int64_t* success;

  _ComposePostService_UploadCreator_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ComposePostService_UploadUrls_args__isset {
  _ComposePostService_UploadUrls_args__isset() : req_id(false), urls(false) {}
  bool req_id :1;
  bool urls :1;
} _ComposePostService_UploadUrls_args__isset;

class ComposePostService_UploadUrls_args {
 public:

  ComposePostService_UploadUrls_args(const ComposePostService_UploadUrls_args&);
  ComposePostService_UploadUrls_args& operator=(const ComposePostService_UploadUrls_args&);
  ComposePostService_UploadUrls_args() : req_id(0) {
  }

	ComposePostService_UploadUrls_args(RandomGenerator* randGen) {
		req_id = randGen->getInt64(RAND_NUM_LIMIT);
		uint32_t iters = randGen->getUInt32(1, 2);
		for(int i=0; i < iters; i++){
			Url url(randGen);
			urls.emplace_back(url);
		}
  }

  virtual ~ComposePostService_UploadUrls_args() throw();
  int64_t req_id;
  std::vector<Url>  urls;

  _ComposePostService_UploadUrls_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_urls(const std::vector<Url> & val);

  bool operator == (const ComposePostService_UploadUrls_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(urls == rhs.urls))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadUrls_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadUrls_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposePostService_UploadUrls_pargs {
 public:


  virtual ~ComposePostService_UploadUrls_pargs() throw();
  const int64_t* req_id;
  const std::vector<Url> * urls;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadUrls_result__isset {
  _ComposePostService_UploadUrls_result__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadUrls_result__isset;

class ComposePostService_UploadUrls_result {
 public:

  ComposePostService_UploadUrls_result(const ComposePostService_UploadUrls_result&);
  ComposePostService_UploadUrls_result& operator=(const ComposePostService_UploadUrls_result&);
  ComposePostService_UploadUrls_result() : success(0) {
  }

	ComposePostService_UploadUrls_result(RandomGenerator *randGen) {
		success = randGen->getInt64(RAND_NUM_LIMIT);
		__isset.success = true;
	}

  virtual ~ComposePostService_UploadUrls_result() throw();
  int64_t success;

  _ComposePostService_UploadUrls_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const ComposePostService_UploadUrls_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadUrls_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadUrls_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadUrls_presult__isset {
  _ComposePostService_UploadUrls_presult__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadUrls_presult__isset;

class ComposePostService_UploadUrls_presult {
 public:


  virtual ~ComposePostService_UploadUrls_presult() throw();
  int64_t* success;

  _ComposePostService_UploadUrls_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ComposePostService_UploadUserMentions_args__isset {
  _ComposePostService_UploadUserMentions_args__isset() : req_id(false), user_mentions(false) {}
  bool req_id :1;
  bool user_mentions :1;
} _ComposePostService_UploadUserMentions_args__isset;

class ComposePostService_UploadUserMentions_args {
 public:

  ComposePostService_UploadUserMentions_args(const ComposePostService_UploadUserMentions_args&);
  ComposePostService_UploadUserMentions_args& operator=(const ComposePostService_UploadUserMentions_args&);
  ComposePostService_UploadUserMentions_args() : req_id(0) {
  }

	ComposePostService_UploadUserMentions_args(RandomGenerator* randGen) {
		req_id = randGen->getInt64(RAND_NUM_LIMIT);
		uint32_t iters = randGen->getUInt32(1, 2);
		for(int i=0; i < iters; i++){
			UserMention um(randGen);
			user_mentions.emplace_back(um);
		}
  }

  virtual ~ComposePostService_UploadUserMentions_args() throw();
  int64_t req_id;
  std::vector<UserMention>  user_mentions;

  _ComposePostService_UploadUserMentions_args__isset __isset;

  void __set_req_id(const int64_t val);

  void __set_user_mentions(const std::vector<UserMention> & val);

  bool operator == (const ComposePostService_UploadUserMentions_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(user_mentions == rhs.user_mentions))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadUserMentions_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadUserMentions_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposePostService_UploadUserMentions_pargs {
 public:


  virtual ~ComposePostService_UploadUserMentions_pargs() throw();
  const int64_t* req_id;
  const std::vector<UserMention> * user_mentions;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadUserMentions_result__isset {
  _ComposePostService_UploadUserMentions_result__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadUserMentions_result__isset;

class ComposePostService_UploadUserMentions_result {
 public:

  ComposePostService_UploadUserMentions_result(const ComposePostService_UploadUserMentions_result&);
  ComposePostService_UploadUserMentions_result& operator=(const ComposePostService_UploadUserMentions_result&);
  ComposePostService_UploadUserMentions_result() : success(0) {
  }

	ComposePostService_UploadUserMentions_result(RandomGenerator *randGen) {
		success = randGen->getInt64(RAND_NUM_LIMIT);
		__isset.success = true;
	}

  virtual ~ComposePostService_UploadUserMentions_result() throw();
  int64_t success;

  _ComposePostService_UploadUserMentions_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const ComposePostService_UploadUserMentions_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ComposePostService_UploadUserMentions_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposePostService_UploadUserMentions_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposePostService_UploadUserMentions_presult__isset {
  _ComposePostService_UploadUserMentions_presult__isset() : success(false) {}
  bool success :1;
} _ComposePostService_UploadUserMentions_presult__isset;

class ComposePostService_UploadUserMentions_presult {
 public:


  virtual ~ComposePostService_UploadUserMentions_presult() throw();
  int64_t* success;

  _ComposePostService_UploadUserMentions_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class ComposePostServiceClient : virtual public ComposePostServiceIf {
 public:
  ComposePostServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposePostServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
	ComposePostServiceClient(RandomGenerator* randGen) {
		initArgs(randGen);
		initResults(randGen);
  }
	ComposePostServiceClient() {
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
  int64_t UploadText(const int64_t req_id, const std::string& text);
  void send_UploadText(const int64_t req_id, const std::string& text);
  int64_t recv_UploadText();
  int64_t UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  void send_UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  int64_t recv_UploadMedia();
  int64_t UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  void send_UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  int64_t recv_UploadUniqueId();
  int64_t UploadCreator(const int64_t req_id, const Creator& creator);
  void send_UploadCreator(const int64_t req_id, const Creator& creator);
  int64_t recv_UploadCreator();
  int64_t UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  void send_UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  int64_t recv_UploadUrls();
  int64_t UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  void send_UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  int64_t recv_UploadUserMentions();

	ComposePostService_UploadText_args *uploadText_args;
	ComposePostService_UploadMedia_args *uploadMedia_args;
	ComposePostService_UploadUniqueId_args *uploadUniqueId_args;
	ComposePostService_UploadCreator_args *uploadCreator_args;
	ComposePostService_UploadUrls_args *uploadUrls_args;
	ComposePostService_UploadUserMentions_args *uploadUserMentions_args;
	void initArgs(RandomGenerator* randGen);
	void send_RandReq(RandomGenerator* randGen);

	void initResults(RandomGenerator* randGen);
	ComposePostService_UploadText_result *uploadText_res;
	ComposePostService_UploadMedia_result *uploadMedia_res;
	ComposePostService_UploadUniqueId_result *uploadUniqueId_res;
	ComposePostService_UploadCreator_result *uploadCreator_res;
	ComposePostService_UploadUrls_result *uploadUrls_res;
	ComposePostService_UploadUserMentions_result *uploadUserMentions_res;
	void FakeUploadUniqueId();

	static void FakeRespGen(ComposePostServiceClient *client, uint64_t fid) {
		switch (fid)
		{
		case FuncType::UPLOAD_UNIQUE_ID:
			client->FakeUploadUniqueId();
			break;
		
		default:
			std::cout << "This is an error, wrong message type (" << fid << ")!" << std::endl;
			exit(1);
			break;
		}	
	}

	static void InitializeFuncMapRedis(FunctionClientMap<ComposePostServiceClient> *f2cmap,
																		 RandomGenerator *randGen,
																		 int num_template_clients,
																		 int num_msg_per_client,
																		 int base_buffer_size) {

		fake_resp_gen_func<ComposePostServiceClient> resp_gen_func = ComposePostServiceClient::FakeRespGen;
		f2cmap->InitMap(resp_gen_func, FuncType::UPLOAD_UNIQUE_ID,
										randGen, num_template_clients, num_msg_per_client, base_buffer_size);
	}

 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ComposePostServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> iface_;  
 private:
  typedef  void (ComposePostServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_UploadText(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadMedia(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadUniqueId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadCreator(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadUrls(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadUserMentions(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ComposePostServiceProcessor(::apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> iface) :
    iface_(iface) {
    processMap_["UploadText"] = &ComposePostServiceProcessor::process_UploadText;
    processMap_["UploadMedia"] = &ComposePostServiceProcessor::process_UploadMedia;
    processMap_["UploadUniqueId"] = &ComposePostServiceProcessor::process_UploadUniqueId;
    processMap_["UploadCreator"] = &ComposePostServiceProcessor::process_UploadCreator;
    processMap_["UploadUrls"] = &ComposePostServiceProcessor::process_UploadUrls;
    processMap_["UploadUserMentions"] = &ComposePostServiceProcessor::process_UploadUserMentions;
  }
	virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);

  virtual ~ComposePostServiceProcessor() {}
};

class ComposePostServiceCerebrosProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> iface_;
 private:
  typedef  uint64_t (ComposePostServiceCerebrosProcessor::*ProcessFunction)(ComposePostServiceClient*);
	typedef ProcessFunction ProcessMap[ComposePostServiceIf::FuncType::SIZE];
  ProcessMap processMap_;
  uint64_t process_UploadText(ComposePostServiceClient* client);
	uint64_t process_UploadMedia(ComposePostServiceClient* client);
	uint64_t process_UploadUniqueId(ComposePostServiceClient* client);
	uint64_t process_UploadCreator(ComposePostServiceClient* client);
	uint64_t process_UploadUrls(ComposePostServiceClient* client);
	uint64_t process_UploadUserMentions(ComposePostServiceClient* client);
 public:
  ComposePostServiceCerebrosProcessor(::apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> iface) :
    iface_(iface) {
		processMap_[ComposePostServiceIf::FuncType::UPLOAD_TEXT] = &ComposePostServiceCerebrosProcessor::process_UploadText;
    processMap_[ComposePostServiceIf::FuncType::UPLOAD_MEDIA] = &ComposePostServiceCerebrosProcessor::process_UploadMedia;
    processMap_[ComposePostServiceIf::FuncType::UPLOAD_UNIQUE_ID] = &ComposePostServiceCerebrosProcessor::process_UploadUniqueId;
    processMap_[ComposePostServiceIf::FuncType::UPLOAD_CREATOR] = &ComposePostServiceCerebrosProcessor::process_UploadCreator;
    processMap_[ComposePostServiceIf::FuncType::UPLOAD_URLS] = &ComposePostServiceCerebrosProcessor::process_UploadUrls;
    processMap_[ComposePostServiceIf::FuncType::UPLOAD_USER_MENTIONS] = &ComposePostServiceCerebrosProcessor::process_UploadUserMentions;
  }
	uint64_t dispatchCall(int funcCode, ComposePostServiceClient* client) {
		if (funcCode >= ComposePostServiceIf::FuncType::SIZE){
			printf("Bad func code (%d) to dispatch in UniqueIdService!\n", funcCode);
			exit(1);
		}

		return (this->*(processMap_[funcCode]))(client);
	}

  virtual ~ComposePostServiceCerebrosProcessor() {}
};

class ComposePostServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ComposePostServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< ComposePostServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< ComposePostServiceIfFactory > handlerFactory_;
};

class ComposePostServiceMultiface : virtual public ComposePostServiceIf {
 public:
  ComposePostServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ComposePostServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> > ifaces_;
  ComposePostServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<ComposePostServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int64_t UploadText(const int64_t req_id, const std::string& text) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadText(req_id, text);
    }
    return ifaces_[i]->UploadText(req_id, text);
  }

  int64_t UploadMedia(const int64_t req_id, const std::vector<Media> & media) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadMedia(req_id, media);
    }
    return ifaces_[i]->UploadMedia(req_id, media);
  }

  int64_t UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUniqueId(req_id, post_id, post_type);
    }
    return ifaces_[i]->UploadUniqueId(req_id, post_id, post_type);
  }

  int64_t UploadCreator(const int64_t req_id, const Creator& creator) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadCreator(req_id, creator);
    }
    return ifaces_[i]->UploadCreator(req_id, creator);
  }

  int64_t UploadUrls(const int64_t req_id, const std::vector<Url> & urls) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUrls(req_id, urls);
    }
    return ifaces_[i]->UploadUrls(req_id, urls);
  }

  int64_t UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadUserMentions(req_id, user_mentions);
    }
    return ifaces_[i]->UploadUserMentions(req_id, user_mentions);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ComposePostServiceConcurrentClient : virtual public ComposePostServiceIf {
 public:
  ComposePostServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposePostServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  int64_t UploadText(const int64_t req_id, const std::string& text);
  int32_t send_UploadText(const int64_t req_id, const std::string& text);
  int64_t recv_UploadText(const int32_t seqid);
  int64_t UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  int32_t send_UploadMedia(const int64_t req_id, const std::vector<Media> & media);
  int64_t recv_UploadMedia(const int32_t seqid);
  int64_t UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  int32_t send_UploadUniqueId(const int64_t req_id, const int64_t post_id, const PostType::type post_type);
  int64_t recv_UploadUniqueId(const int32_t seqid);
  int64_t UploadCreator(const int64_t req_id, const Creator& creator);
  int32_t send_UploadCreator(const int64_t req_id, const Creator& creator);
  int64_t recv_UploadCreator(const int32_t seqid);
  int64_t UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  int32_t send_UploadUrls(const int64_t req_id, const std::vector<Url> & urls);
  int64_t recv_UploadUrls(const int32_t seqid);
  int64_t UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  int32_t send_UploadUserMentions(const int64_t req_id, const std::vector<UserMention> & user_mentions);
  int64_t recv_UploadUserMentions(const int32_t seqid);
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