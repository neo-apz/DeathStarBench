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

service UniqueIdService {
  i64 UploadUniqueId (
      1: i64 req_id,
      2: PostType post_type
  )
}

service ComposePostService {
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

service FakeRedis {
  void HSetCreator(
      1: i64 req_id,
      2: string field,
      3: Creator creator
  )

  void HSetText(
      1: i64 req_id,
      2: string field,
      3: string text
  )

  void HSetMedia(
      1: i64 req_id,
      2: string field,
      3: list<Media> media
  )

  void HSetPostId(
      1: i64 req_id,
      2: string field,
      3: i64 post_id
  )

  void HSetPostType(
      1: i64 req_id,
      2: string field,
      3: PostType post_type
  )

  void HSetUrls(
      1: i64 req_id,
      2: string field,
      3: list<Url> urls
  )

  void HSetUserMentions(
      1: i64 req_id,
      2: string field,
      3: list<UserMention> user_mentions
  )

  Creator HGetCreator(
      1: i64 req_id,
      2: string field
  )

  string HGetText(
      1: i64 req_id,
      2: string field
  )

  list<Media> HGetMedia(
      1: i64 req_id,
      2: string field
  )

  i64 HGetPostId(
      1: i64 req_id,
      2: string field
  )

  PostType HGetPostType(
      1: i64 req_id,
      2: string field
  )

  list<Url> HGetUrls(
      1: i64 req_id,
      2: string field
  )

  list<UserMention> HGetUserMentions(
      1: i64 req_id,
      2: string field
  )

  i64 HIncrBy(
      1: i64 key,
      2: string field,
      3: i64 value
  )
}

service FakeRabbitmq {
  void UploadHomeTimeline(
    1: i64 req_id,
    2: i64 post_id,
    3: i64 user_id,
    4: i64 timestamp,
    5: list<i64> user_mentions_id
  )
}

service UserTimelineService {
  void WriteUserTimeline(
    1: i64 req_id,
    2: i64 post_id,
    3: i64 user_id,
    4: i64 timestamp
  ) 

  list<Post> ReadUserTimeline(
    1: i64 req_id,
    2: i64 user_id,
    3: i32 start,
    4: i32 stop
  ) 
}

service PostStorageService {
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

service MyTextService {
  void UploadText (
      1: i64 req_id,
      2: string text
      // 3: map<string, string> carrier
  ) 
}

service FakeMemcached {
  i64 UserCached (
		1: string username
  )

	i64 GetUserId (
		1: string username
  )

	void InsertUserId (
		1: string username
		2: i64 user_id
  )

	User LoginCached (
		1: string username
  )

	void InsertUser (
		1: string username
		2: User user
  )
}

service FakeMongo {
  bool UserExists (
		1: string username
  )

	void InsertUser (
		1: User user
  )

	i64 CreatorExists (
		1: string username
  )

	User GetUser (
		1: string username
  ) 
}

service UserService {
  bool RegisterUser (
      1: i64 req_id,
      2: string first_name,
      3: string last_name,
      4: string username,
      5: string password
  ) 

	bool RegisterUserWithId (
			1: i64 req_id,
			2: string first_name,
			3: string last_name,
			4: string username,
			5: string password,
			6: i64 user_id
	) 

  string Login(
      1: i64 req_id,
      2: string username,
      3: string password
  ) 

  void UploadCreatorWithUserId(
      1: i64 req_id,
      2: i64 user_id,
      3: string username
  ) 

  bool UploadCreatorWithUsername(
      1: i64 req_id,
      2: string username
  ) 

  i64 GetUserId(
      1: i64 req_id,
      2: string username
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

service MySocialGraphService{
  list<i64> GetFollowers(
      1: i64 req_id,
      2: i64 user_id
  ) 

  list<i64> GetFollowees(
      1: i64 req_id,
      2: i64 user_id
  ) 

  i64 Follow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
  ) 

  i64 Unfollow(
      1: i64 req_id,
      2: i64 user_id,
      3: i64 followee_id
   ) 

  i64 FollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
  ) 

  i64 UnfollowWithUsername(
      1: i64 req_id,
      2: string user_usernmae,
      3: string followee_username
  ) 

  i64 InsertUser(
     1: i64 req_id,
     2: i64 user_id
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