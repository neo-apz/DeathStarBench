namespace cpp my_social_network
namespace py my_social_network
namespace lua my_social_network

struct User {
    1: i64 user_id;
    2: string first_name;
    3: string last_name;
    4: string username;
    5: string password_hashed;
    6: string salt;
}

enum ErrorCode {
  SE_CONNPOOL_TIMEOUT,
  SE_THRIFT_CONN_ERROR,
  SE_UNAUTHORIZED,
  SE_MEMCACHED_ERROR,
  SE_MONGODB_ERROR,
  SE_REDIS_ERROR,
  SE_THRIFT_HANDLER_ERROR,
  SE_RABBITMQ_CONN_ERROR
}

exception ServiceException {
    1: ErrorCode errorCode;
    2: string message;
}

enum PostType {
  POST,
  REPOST,
  REPLY,
  DM
}

struct Media {
  1: i64 media_id;
  2: string media_type;
}

struct Url {
  1: string shortened_url;
  2: string expanded_url;
}

struct UserMention {
  1: i64 user_id;
  2: string username;
}

struct Creator {
  1: i64 user_id;
  2: string username;
}

struct Post {
  1: i64 post_id;
  2: Creator creator;
  3: i64 req_id;
  4: string text;
  5: list<UserMention> user_mentions;
  6: list<Media> media;
  7: list<Url> urls;
  8: i64 timestamp;
  9: PostType post_type;
}

service FakeUniqueIdService {
  void UploadUniqueId (
      1: i64 req_id,
      2: PostType post_type
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service MyUniqueIdService {
  void UploadUniqueId (
      1: i64 req_id,
      2: PostType post_type
  ) throws (1: ServiceException se)
}

service FakeTextService {
  void UploadText (
      1: i64 req_id,
      2: string text
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service MyTextService {
  void UploadText (
      1: i64 req_id,
      2: string text
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service FakeUserService {
  void RegisterUser (
      1: i64 req_id,
      2: string first_name,
      3: string last_name,
      4: string username,
      5: string password
      // 6: map<string, string> carrier
  ) throws (1: ServiceException se)

    void RegisterUserWithId (
        1: i64 req_id,
        2: string first_name,
        3: string last_name,
        4: string username,
        5: string password,
        6: i64 user_id
        // 7: map<string, string> carrier
    ) throws (1: ServiceException se)

  string Login(
      1: i64 req_id,
      2: string username,
      3: string password
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadCreatorWithUserId(
      1: i64 req_id,
      2: i64 user_id,
      3: string username
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadCreatorWithUsername(
      1: i64 req_id,
      2: string username
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  i64 GetUserId(
      1: i64 req_id,
      2: string username
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}


service MyUserService {
  void RegisterUser (
      1: i64 req_id,
      2: string first_name,
      3: string last_name,
      4: string username,
      5: string password
      // 6: map<string, string> carrier
  ) throws (1: ServiceException se)

    void RegisterUserWithId (
        1: i64 req_id,
        2: string first_name,
        3: string last_name,
        4: string username,
        5: string password,
        6: i64 user_id
        // 7: map<string, string> carrier
    ) throws (1: ServiceException se)

  string Login(
      1: i64 req_id,
      2: string username,
      3: string password
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadCreatorWithUserId(
      1: i64 req_id,
      2: i64 user_id,
      3: string username
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadCreatorWithUsername(
      1: i64 req_id,
      2: string username
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  i64 GetUserId(
      1: i64 req_id,
      2: string username
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service FakeComposePostService {
  void UploadText(
      1: i64 req_id,
      2: string text
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadMedia(
      1: i64 req_id,
      2: list<Media> media
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadUniqueId(
      1: i64 req_id,
      2: i64 post_id,
      3: PostType post_type
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadCreator(
      1: i64 req_id,
      2: Creator creator
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadUrls(
      1: i64 req_id,
      2: list<Url> urls
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UploadUserMentions(
      1: i64 req_id,
      2: list<UserMention> user_mentions
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service MyComposePostService {
  void UploadText(
      1: i64 req_id,
      2: string text
  ) throws (1: ServiceException se)

  void UploadMedia(
      1: i64 req_id,
      2: list<Media> media
  ) throws (1: ServiceException se)

  void UploadUniqueId(
      1: i64 req_id,
      2: i64 post_id,
      3: PostType post_type
  ) throws (1: ServiceException se)

  void UploadCreator(
      1: i64 req_id,
      2: Creator creator
  ) throws (1: ServiceException se)

  void UploadUrls(
      1: i64 req_id,
      2: list<Url> urls
  ) throws (1: ServiceException se)

  void UploadUserMentions(
      1: i64 req_id,
      2: list<UserMention> user_mentions
  ) throws (1: ServiceException se)
}

service MyPostStorageService {
  void StorePost(
    1: i64 req_id,
    2: Post post
    // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  Post ReadPost(
    1: i64 req_id,
    2: i64 post_id
    // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  list<Post> ReadPosts(
    1: i64 req_id,
    2: list<i64> post_ids
    // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service FakePostStorageService {
  void StorePost(
    1: i64 req_id,
    2: Post post
    // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  Post ReadPost(
    1: i64 req_id,
    2: i64 post_id
    // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  list<Post> ReadPosts(
    1: i64 req_id,
    2: list<i64> post_ids
    // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service FakeHomeTimelineService {
  list<Post> ReadHomeTimeline(
    1: i64 req_id,
    2: i64 user_id,
    3: i32 start,
    4: i32 stop
    // 5: map<string, string> carrier
  ) throws (1: ServiceException se)
}


service MyHomeTimelineService {
  list<Post> ReadHomeTimeline(
    1: i64 req_id,
    2: i64 user_id,
    3: i32 start,
    4: i32 stop
    // 5: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service MyUserTimelineService {
  void WriteUserTimeline(
    1: i64 req_id,
    2: i64 post_id,
    3: i64 user_id,
    4: i64 timestamp
    // 5: map<string, string> carrier
  ) throws (1: ServiceException se)

  list<Post> ReadUserTimeline(
    1: i64 req_id,
    2: i64 user_id,
    3: i32 start,
    4: i32 stop
    // 5: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service FakeUserTimelineService {
  void WriteUserTimeline(
    1: i64 req_id,
    2: i64 post_id,
    3: i64 user_id,
    4: i64 timestamp
    // 5: map<string, string> carrier
  ) throws (1: ServiceException se)

  list<Post> ReadUserTimeline(
    1: i64 req_id,
    2: i64 user_id,
    3: i32 start,
    4: i32 stop
    // 5: map<string, string> carrier
  ) throws (1: ServiceException se)
}


service FakeSocialGraphService{
  list<i64> GetFollowers(
      1: i64 req_id,
      2: i64 user_id
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  list<i64> GetFollowees(
      1: i64 req_id,
      2: i64 user_id
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  void Follow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void Unfollow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
      // 4: map<string, string> carrier
   ) throws (1: ServiceException se)

  void FollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UnfollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void InsertUser(
     1: i64 req_id,
     2: i64 user_id
    //  3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service MySocialGraphService{
  list<i64> GetFollowers(
      1: i64 req_id,
      2: i64 user_id
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  list<i64> GetFollowees(
      1: i64 req_id,
      2: i64 user_id
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

  void Follow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void Unfollow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
      // 4: map<string, string> carrier
   ) throws (1: ServiceException se)

  void FollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void UnfollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)

  void InsertUser(
     1: i64 req_id,
     2: i64 user_id
    //  3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service MyUserMentionService {
  void UploadUserMentions(
      1: i64 req_id,
      2: list<string> usernames
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}


service FakeUserMentionService {
  void UploadUserMentions(
      1: i64 req_id,
      2: list<string> usernames
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service FakeUrlShortenService {
  list<string> UploadUrls(
      1: i64 req_id,
      2: list<string> urls
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

    list<string> GetExtendedUrls(
        1: i64 req_id,
        2: list<string> shortened_urls
        // 3: map<string, string> carrier
    ) throws (1: ServiceException se)
}

service MyUrlShortenService {
  list<string> UploadUrls(
      1: i64 req_id,
      2: list<string> urls
      // 3: map<string, string> carrier
  ) throws (1: ServiceException se)

    list<string> GetExtendedUrls(
        1: i64 req_id,
        2: list<string> shortened_urls
        // 3: map<string, string> carrier
    ) throws (1: ServiceException se)
}

service MyMediaService {
  void UploadMedia(
      1: i64 req_id,
      2: list<string> media_types,
      3: list<i64> media_ids
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)
}

service FakeMediaService {
  void UploadMedia(
      1: i64 req_id,
      2: list<string> media_types,
      3: list<i64> media_ids
      // 4: map<string, string> carrier
  ) throws (1: ServiceException se)
}