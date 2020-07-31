
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