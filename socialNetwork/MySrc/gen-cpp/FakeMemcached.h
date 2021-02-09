/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef FakeMemcached_H
#define FakeMemcached_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "my_social_network_types.h"

#include <RandomGenerator.h>
#include <FunctionClientMap.h>

#include <iostream>

namespace my_social_network {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class FakeMemcachedIf {
 public:
  virtual ~FakeMemcachedIf() {}
  virtual int64_t UserCached(const std::string& username) = 0;
  virtual int64_t GetUserId(const std::string& username) = 0;
  virtual void InsertUserId(const std::string& username, const int64_t user_id) = 0;
  virtual void LoginCached(User& _return, const std::string& username) = 0;
  virtual void InsertUser(const std::string& username, const User& user) = 0;

	struct FuncType {
  enum type {
    USER_CACHED = 0,
    GET_USER_ID = 1,
    INSERT_USER_ID = 2,
    LOGIN_CACHED = 3,
		INSERT_USER = 4,

    SIZE = 5
  	};
	};
};

class FakeMemcachedIfFactory {
 public:
  typedef FakeMemcachedIf Handler;

  virtual ~FakeMemcachedIfFactory() {}

  virtual FakeMemcachedIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(FakeMemcachedIf* /* handler */) = 0;
};

class FakeMemcachedIfSingletonFactory : virtual public FakeMemcachedIfFactory {
 public:
  FakeMemcachedIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<FakeMemcachedIf>& iface) : iface_(iface) {}
  virtual ~FakeMemcachedIfSingletonFactory() {}

  virtual FakeMemcachedIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(FakeMemcachedIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeMemcachedIf> iface_;
};

class FakeMemcachedNull : virtual public FakeMemcachedIf {
 public:
  virtual ~FakeMemcachedNull() {}
  int64_t UserCached(const std::string& /* username */) {
    int64_t _return = 0;
    return _return;
  }
  int64_t GetUserId(const std::string& /* username */) {
    int64_t _return = 0;
    return _return;
  }
  void InsertUserId(const std::string& /* username */, const int64_t /* user_id */) {
    return;
  }
  void LoginCached(User& /* _return */, const std::string& /* username */) {
    return;
  }
  void InsertUser(const std::string& /* username */, const User& /* user */) {
    return;
  }
};

typedef struct _FakeMemcached_UserCached_args__isset {
  _FakeMemcached_UserCached_args__isset() : username(false) {}
  bool username :1;
} _FakeMemcached_UserCached_args__isset;

class FakeMemcached_UserCached_args {
 public:

  FakeMemcached_UserCached_args(const FakeMemcached_UserCached_args&);
  FakeMemcached_UserCached_args& operator=(const FakeMemcached_UserCached_args&);
  FakeMemcached_UserCached_args() : username() {
  }

  virtual ~FakeMemcached_UserCached_args() throw();
  std::string username;

  _FakeMemcached_UserCached_args__isset __isset;

  void __set_username(const std::string& val);

  bool operator == (const FakeMemcached_UserCached_args & rhs) const
  {
    if (!(username == rhs.username))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_UserCached_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_UserCached_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_UserCached_pargs {
 public:


  virtual ~FakeMemcached_UserCached_pargs() throw();
  const std::string* username;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMemcached_UserCached_result__isset {
  _FakeMemcached_UserCached_result__isset() : success(false) {}
  bool success :1;
} _FakeMemcached_UserCached_result__isset;

class FakeMemcached_UserCached_result {
 public:

  FakeMemcached_UserCached_result(const FakeMemcached_UserCached_result&);
  FakeMemcached_UserCached_result& operator=(const FakeMemcached_UserCached_result&);
  FakeMemcached_UserCached_result() : success(0) {
  }

	FakeMemcached_UserCached_result(RandomGenerator *randGen) {
		uint64_t dice = randGen->getInt64(100);

		if (dice < 90){ // Cached!
			success = randGen->getInt64(RAND_NUM_LIMIT);
		}
		else { // Not Cached!
			success = -1;
		}

		__isset.success = true;
	}

  virtual ~FakeMemcached_UserCached_result() throw();
  int64_t success;

  _FakeMemcached_UserCached_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const FakeMemcached_UserCached_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_UserCached_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_UserCached_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMemcached_UserCached_presult__isset {
  _FakeMemcached_UserCached_presult__isset() : success(false) {}
  bool success :1;
} _FakeMemcached_UserCached_presult__isset;

class FakeMemcached_UserCached_presult {
 public:


  virtual ~FakeMemcached_UserCached_presult() throw();
  int64_t* success;

  _FakeMemcached_UserCached_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeMemcached_GetUserId_args__isset {
  _FakeMemcached_GetUserId_args__isset() : username(false) {}
  bool username :1;
} _FakeMemcached_GetUserId_args__isset;

class FakeMemcached_GetUserId_args {
 public:

  FakeMemcached_GetUserId_args(const FakeMemcached_GetUserId_args&);
  FakeMemcached_GetUserId_args& operator=(const FakeMemcached_GetUserId_args&);
  FakeMemcached_GetUserId_args() : username() {
  }

  virtual ~FakeMemcached_GetUserId_args() throw();
  std::string username;

  _FakeMemcached_GetUserId_args__isset __isset;

  void __set_username(const std::string& val);

  bool operator == (const FakeMemcached_GetUserId_args & rhs) const
  {
    if (!(username == rhs.username))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_GetUserId_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_GetUserId_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_GetUserId_pargs {
 public:


  virtual ~FakeMemcached_GetUserId_pargs() throw();
  const std::string* username;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMemcached_GetUserId_result__isset {
  _FakeMemcached_GetUserId_result__isset() : success(false) {}
  bool success :1;
} _FakeMemcached_GetUserId_result__isset;

class FakeMemcached_GetUserId_result {
 public:

  FakeMemcached_GetUserId_result(const FakeMemcached_GetUserId_result&);
  FakeMemcached_GetUserId_result& operator=(const FakeMemcached_GetUserId_result&);
  FakeMemcached_GetUserId_result() : success(0) {
  }

	FakeMemcached_GetUserId_result(RandomGenerator *randGen) {
		uint64_t dice = randGen->getInt64(100);

		if (dice < 90){ // Cached!
			success = randGen->getInt64(RAND_NUM_LIMIT);
		}
		else { // Not Cached!
			success = -1;
		}

		__isset.success = true;
	}

  virtual ~FakeMemcached_GetUserId_result() throw();
  int64_t success;

  _FakeMemcached_GetUserId_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const FakeMemcached_GetUserId_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_GetUserId_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_GetUserId_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMemcached_GetUserId_presult__isset {
  _FakeMemcached_GetUserId_presult__isset() : success(false) {}
  bool success :1;
} _FakeMemcached_GetUserId_presult__isset;

class FakeMemcached_GetUserId_presult {
 public:


  virtual ~FakeMemcached_GetUserId_presult() throw();
  int64_t* success;

  _FakeMemcached_GetUserId_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeMemcached_InsertUserId_args__isset {
  _FakeMemcached_InsertUserId_args__isset() : username(false), user_id(false) {}
  bool username :1;
  bool user_id :1;
} _FakeMemcached_InsertUserId_args__isset;

class FakeMemcached_InsertUserId_args {
 public:

  FakeMemcached_InsertUserId_args(const FakeMemcached_InsertUserId_args&);
  FakeMemcached_InsertUserId_args& operator=(const FakeMemcached_InsertUserId_args&);
  FakeMemcached_InsertUserId_args() : username(), user_id(0) {
  }

  virtual ~FakeMemcached_InsertUserId_args() throw();
  std::string username;
  int64_t user_id;

  _FakeMemcached_InsertUserId_args__isset __isset;

  void __set_username(const std::string& val);

  void __set_user_id(const int64_t val);

  bool operator == (const FakeMemcached_InsertUserId_args & rhs) const
  {
    if (!(username == rhs.username))
      return false;
    if (!(user_id == rhs.user_id))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_InsertUserId_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_InsertUserId_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_InsertUserId_pargs {
 public:


  virtual ~FakeMemcached_InsertUserId_pargs() throw();
  const std::string* username;
  const int64_t* user_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_InsertUserId_result {
 public:

  FakeMemcached_InsertUserId_result(const FakeMemcached_InsertUserId_result&);
  FakeMemcached_InsertUserId_result& operator=(const FakeMemcached_InsertUserId_result&);
  FakeMemcached_InsertUserId_result() {
  }

	FakeMemcached_InsertUserId_result(RandomGenerator *randGen) {
	}

  virtual ~FakeMemcached_InsertUserId_result() throw();

  bool operator == (const FakeMemcached_InsertUserId_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const FakeMemcached_InsertUserId_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_InsertUserId_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_InsertUserId_presult {
 public:


  virtual ~FakeMemcached_InsertUserId_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeMemcached_LoginCached_args__isset {
  _FakeMemcached_LoginCached_args__isset() : username(false) {}
  bool username :1;
} _FakeMemcached_LoginCached_args__isset;

class FakeMemcached_LoginCached_args {
 public:

  FakeMemcached_LoginCached_args(const FakeMemcached_LoginCached_args&);
  FakeMemcached_LoginCached_args& operator=(const FakeMemcached_LoginCached_args&);
  FakeMemcached_LoginCached_args() : username() {
  }

  virtual ~FakeMemcached_LoginCached_args() throw();
  std::string username;

  _FakeMemcached_LoginCached_args__isset __isset;

  void __set_username(const std::string& val);

  bool operator == (const FakeMemcached_LoginCached_args & rhs) const
  {
    if (!(username == rhs.username))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_LoginCached_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_LoginCached_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_LoginCached_pargs {
 public:


  virtual ~FakeMemcached_LoginCached_pargs() throw();
  const std::string* username;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMemcached_LoginCached_result__isset {
  _FakeMemcached_LoginCached_result__isset() : success(false) {}
  bool success :1;
} _FakeMemcached_LoginCached_result__isset;

class FakeMemcached_LoginCached_result {
 public:

  FakeMemcached_LoginCached_result(const FakeMemcached_LoginCached_result&);
  FakeMemcached_LoginCached_result& operator=(const FakeMemcached_LoginCached_result&);
  FakeMemcached_LoginCached_result() {
  }

	FakeMemcached_LoginCached_result(RandomGenerator *randGen) {
		uint64_t dice = randGen->getInt64(100);

		if (dice < 90){ // Cached!
			success = User(randGen);
		}
		else { // Not Cached!
			success.user_id = -1;
		}

		__isset.success = true;
	}

  virtual ~FakeMemcached_LoginCached_result() throw();
  User success;

  _FakeMemcached_LoginCached_result__isset __isset;

  void __set_success(const User& val);

  bool operator == (const FakeMemcached_LoginCached_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_LoginCached_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_LoginCached_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FakeMemcached_LoginCached_presult__isset {
  _FakeMemcached_LoginCached_presult__isset() : success(false) {}
  bool success :1;
} _FakeMemcached_LoginCached_presult__isset;

class FakeMemcached_LoginCached_presult {
 public:


  virtual ~FakeMemcached_LoginCached_presult() throw();
  User* success;

  _FakeMemcached_LoginCached_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _FakeMemcached_InsertUser_args__isset {
  _FakeMemcached_InsertUser_args__isset() : username(false), user(false) {}
  bool username :1;
  bool user :1;
} _FakeMemcached_InsertUser_args__isset;

class FakeMemcached_InsertUser_args {
 public:

  FakeMemcached_InsertUser_args(const FakeMemcached_InsertUser_args&);
  FakeMemcached_InsertUser_args& operator=(const FakeMemcached_InsertUser_args&);
  FakeMemcached_InsertUser_args() : username() {
  }

  virtual ~FakeMemcached_InsertUser_args() throw();
  std::string username;
  User user;

  _FakeMemcached_InsertUser_args__isset __isset;

  void __set_username(const std::string& val);

  void __set_user(const User& val);

  bool operator == (const FakeMemcached_InsertUser_args & rhs) const
  {
    if (!(username == rhs.username))
      return false;
    if (!(user == rhs.user))
      return false;
    return true;
  }
  bool operator != (const FakeMemcached_InsertUser_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_InsertUser_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_InsertUser_pargs {
 public:


  virtual ~FakeMemcached_InsertUser_pargs() throw();
  const std::string* username;
  const User* user;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_InsertUser_result {
 public:

  FakeMemcached_InsertUser_result(const FakeMemcached_InsertUser_result&);
  FakeMemcached_InsertUser_result& operator=(const FakeMemcached_InsertUser_result&);
  FakeMemcached_InsertUser_result() {
  }

	FakeMemcached_InsertUser_result(RandomGenerator *randGen) {
	}

  virtual ~FakeMemcached_InsertUser_result() throw();

  bool operator == (const FakeMemcached_InsertUser_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const FakeMemcached_InsertUser_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FakeMemcached_InsertUser_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FakeMemcached_InsertUser_presult {
 public:


  virtual ~FakeMemcached_InsertUser_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class FakeMemcachedClient : virtual public FakeMemcachedIf {
 public:
  FakeMemcachedClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FakeMemcachedClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
	FakeMemcachedClient(){
		
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
  int64_t UserCached(const std::string& username);
  void send_UserCached(const std::string& username);
  int64_t recv_UserCached();
  int64_t GetUserId(const std::string& username);
  void send_GetUserId(const std::string& username);
  int64_t recv_GetUserId();
  void InsertUserId(const std::string& username, const int64_t user_id);
  void send_InsertUserId(const std::string& username, const int64_t user_id);
  void recv_InsertUserId();
  void LoginCached(User& _return, const std::string& username);
  void send_LoginCached(const std::string& username);
  void recv_LoginCached(User& _return);
  void InsertUser(const std::string& username, const User& user);
  void send_InsertUser(const std::string& username, const User& user);
  void recv_InsertUser();

	void initResults(RandomGenerator* randGen);
	FakeMemcached_UserCached_result *userCached_res;
	FakeMemcached_GetUserId_result *getUserId_res;
	FakeMemcached_InsertUserId_result *insertUserId_res;
	FakeMemcached_LoginCached_result *loginCached_res;
	FakeMemcached_InsertUser_result *insertUser_res;
	void FakeUserCached();
	void FakeGetUserId();
	void FakeInsertUserId();
	void FakeLoginCached();
	void FakeInsertUser();

	static void FakeRespGen(FakeMemcachedClient *client, uint64_t fid) {
		switch (fid)
		{
		case FuncType::USER_CACHED:
			client->FakeUserCached();
			break;

		case FuncType::GET_USER_ID:
			client->FakeGetUserId();
			break;

		case FuncType::INSERT_USER_ID:
			client->FakeInsertUserId();
			break;

		case FuncType::LOGIN_CACHED:
			client->FakeLoginCached();
			break;

		case FuncType::INSERT_USER:
			client->FakeInsertUser();
			break;		

		default:
			std::cout << "This is an error, wrong message type in FakeMongo's FakeRespGen (" << fid << ")!" << std::endl;
			exit(1);
			break;
		}	
	}
	
	static void InitializeFuncMapMemcached(FunctionClientMap<FakeMemcachedClient> *f2cmap,
															RandomGenerator *randGen,
															int num_template_clients,
															int num_msg_per_client,
															int base_buffer_size) {

		fake_resp_gen_func<FakeMemcachedClient> resp_gen_func = FakeMemcachedClient::FakeRespGen;
		
		f2cmap->InitMap(resp_gen_func, FuncType::USER_CACHED,
										randGen, num_template_clients, num_msg_per_client, base_buffer_size);

		f2cmap->InitMap(resp_gen_func, FuncType::GET_USER_ID,
										randGen, num_template_clients, num_msg_per_client, base_buffer_size);

		f2cmap->InitMap(resp_gen_func, FuncType::INSERT_USER_ID,
										randGen, num_template_clients, num_msg_per_client, base_buffer_size);

		f2cmap->InitMap(resp_gen_func, FuncType::LOGIN_CACHED,
										randGen, num_template_clients, num_msg_per_client, base_buffer_size);										
		
		f2cmap->InitMap(resp_gen_func, FuncType::INSERT_USER,
										randGen, num_template_clients, num_msg_per_client, base_buffer_size);										
		}	

 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class FakeMemcachedProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<FakeMemcachedIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (FakeMemcachedProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_UserCached(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetUserId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_InsertUserId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_LoginCached(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_InsertUser(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  FakeMemcachedProcessor(::apache::thrift::stdcxx::shared_ptr<FakeMemcachedIf> iface) :
    iface_(iface) {
    processMap_["UserCached"] = &FakeMemcachedProcessor::process_UserCached;
    processMap_["GetUserId"] = &FakeMemcachedProcessor::process_GetUserId;
    processMap_["InsertUserId"] = &FakeMemcachedProcessor::process_InsertUserId;
    processMap_["LoginCached"] = &FakeMemcachedProcessor::process_LoginCached;
    processMap_["InsertUser"] = &FakeMemcachedProcessor::process_InsertUser;
  }

  virtual ~FakeMemcachedProcessor() {}
};

class FakeMemcachedProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  FakeMemcachedProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< FakeMemcachedIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< FakeMemcachedIfFactory > handlerFactory_;
};

class FakeMemcachedMultiface : virtual public FakeMemcachedIf {
 public:
  FakeMemcachedMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<FakeMemcachedIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~FakeMemcachedMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<FakeMemcachedIf> > ifaces_;
  FakeMemcachedMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<FakeMemcachedIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int64_t UserCached(const std::string& username) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UserCached(username);
    }
    return ifaces_[i]->UserCached(username);
  }

  int64_t GetUserId(const std::string& username) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetUserId(username);
    }
    return ifaces_[i]->GetUserId(username);
  }

  void InsertUserId(const std::string& username, const int64_t user_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->InsertUserId(username, user_id);
    }
    ifaces_[i]->InsertUserId(username, user_id);
  }

  void LoginCached(User& _return, const std::string& username) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->LoginCached(_return, username);
    }
    ifaces_[i]->LoginCached(_return, username);
    return;
  }

  void InsertUser(const std::string& username, const User& user) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->InsertUser(username, user);
    }
    ifaces_[i]->InsertUser(username, user);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class FakeMemcachedConcurrentClient : virtual public FakeMemcachedIf {
 public:
  FakeMemcachedConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FakeMemcachedConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  int64_t UserCached(const std::string& username);
  int32_t send_UserCached(const std::string& username);
  int64_t recv_UserCached(const int32_t seqid);
  int64_t GetUserId(const std::string& username);
  int32_t send_GetUserId(const std::string& username);
  int64_t recv_GetUserId(const int32_t seqid);
  void InsertUserId(const std::string& username, const int64_t user_id);
  int32_t send_InsertUserId(const std::string& username, const int64_t user_id);
  void recv_InsertUserId(const int32_t seqid);
  void LoginCached(User& _return, const std::string& username);
  int32_t send_LoginCached(const std::string& username);
  void recv_LoginCached(User& _return, const int32_t seqid);
  void InsertUser(const std::string& username, const User& user);
  int32_t send_InsertUser(const std::string& username, const User& user);
  void recv_InsertUser(const int32_t seqid);
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
