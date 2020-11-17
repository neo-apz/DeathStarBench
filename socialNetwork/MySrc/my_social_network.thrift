namespace cpp my_social_network

struct User {
    1: i64 user_id;
    2: string first_name;
    3: string last_name;
    4: string username;
    5: string password_hashed;
    6: string salt;
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

service MyUniqueIdService {
  i64 UploadUniqueId (
      1: i64 req_id,
      2: PostType post_type
  )
}

service MyTextService {
  void UploadText (
      1: i64 req_id,
      2: string text
      // 3: map<string, string> carrier
  ) 
}

service MyUserService {
  void RegisterUser (
      1: i64 req_id,
      2: string first_name,
      3: string last_name,
      4: string username,
      5: string password
      // 6: map<string, string> carrier
  ) 

    void RegisterUserWithId (
        1: i64 req_id,
        2: string first_name,
        3: string last_name,
        4: string username,
        5: string password,
        6: i64 user_id
        // 7: map<string, string> carrier
    ) 

  string Login(
      1: i64 req_id,
      2: string username,
      3: string password
      // 4: map<string, string> carrier
  ) 

  void UploadCreatorWithUserId(
      1: i64 req_id,
      2: i64 user_id,
      3: string username
      // 4: map<string, string> carrier
  ) 

  void UploadCreatorWithUsername(
      1: i64 req_id,
      2: string username
      // 3: map<string, string> carrier
  ) 

  i64 GetUserId(
      1: i64 req_id,
      2: string username
      // 3: map<string, string> carrier
  ) 
}

service MyComposePostService {
  i64 UploadText(
      1: i64 req_id,
      2: string text
  ) 

  i64 UploadMedia(
      1: i64 req_id,
      2: list<Media> media
  ) 

  i64 UploadUniqueId(
      1: i64 req_id,
      2: i64 post_id,
      3: PostType post_type
  ) 

  i64 UploadCreator(
      1: i64 req_id,
      2: Creator creator
  ) 

  i64 UploadUrls(
      1: i64 req_id,
      2: list<Url> urls
  ) 

  i64 UploadUserMentions(
      1: i64 req_id,
      2: list<UserMention> user_mentions
  ) 
}

service MyPostStorageService {
  void StorePost(
    1: i64 req_id,
    2: Post post
    // 3: map<string, string> carrier
  ) 

  Post ReadPost(
    1: i64 req_id,
    2: i64 post_id
    // 3: map<string, string> carrier
  ) 

  list<Post> ReadPosts(
    1: i64 req_id,
    2: list<i64> post_ids
    // 3: map<string, string> carrier
  ) 
}

service MyHomeTimelineService {
  list<Post> ReadHomeTimeline(
    1: i64 req_id,
    2: i64 user_id,
    3: i32 start,
    4: i32 stop
    // 5: map<string, string> carrier
  ) 
}

service MyUserTimelineService {
  void WriteUserTimeline(
    1: i64 req_id,
    2: i64 post_id,
    3: i64 user_id,
    4: i64 timestamp
    // 5: map<string, string> carrier
  ) 

  list<Post> ReadUserTimeline(
    1: i64 req_id,
    2: i64 user_id,
    3: i32 start,
    4: i32 stop
    // 5: map<string, string> carrier
  ) 
}

service MySocialGraphService{
  list<i64> GetFollowers(
      1: i64 req_id,
      2: i64 user_id
      // 3: map<string, string> carrier
  ) 

  list<i64> GetFollowees(
      1: i64 req_id,
      2: i64 user_id
      // 3: map<string, string> carrier
  ) 

  void Follow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
      // 4: map<string, string> carrier
  ) 

  void Unfollow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
      // 4: map<string, string> carrier
   ) 

  void FollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
      // 4: map<string, string> carrier
  ) 

  void UnfollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
      // 4: map<string, string> carrier
  ) 

  void InsertUser(
     1: i64 req_id,
     2: i64 user_id
    //  3: map<string, string> carrier
  ) 
}

service MyUserMentionService {
  void UploadUserMentions(
      1: i64 req_id,
      2: list<string> usernames
      // 3: map<string, string> carrier
  ) 
}

service MyUrlShortenService {
  list<string> UploadUrls(
      1: i64 req_id,
      2: list<string> urls
      // 3: map<string, string> carrier
  ) 

    list<string> GetExtendedUrls(
        1: i64 req_id,
        2: list<string> shortened_urls
        // 3: map<string, string> carrier
    ) 
}

service MyMediaService {
  void UploadMedia(
      1: i64 req_id,
      2: list<string> media_types,
      3: list<i64> media_ids
      // 4: map<string, string> carrier
  ) 
}