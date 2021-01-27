#include "FunctionClientMap.h"

#include "../gen-cpp/ComposePostService.h"
#include "../gen-cpp/FakeRedis.h"
#include "../gen-cpp/FakeRabbitmq.h"
#include "../gen-cpp/PostStorageService.h"
#include "../gen-cpp/UserTimelineService.h"

#include <RandomGenerator.h>

void InitializeFuncMapComposePost(FunctionClientMap<ComposePostServiceClient> *f2cmap,
																	RandomGenerator *randGen,
																	int num_template_clients,
																	int num_msg_per_client,
																	int base_buffer_size) {
	
	uint64_t buffer_size = num_msg_per_client * base_buffer_size;

	MyThriftClient<ComposePostServiceClient>** clients = new MyThriftClient<ComposePostServiceClient>*[num_template_clients];

	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<ComposePostServiceClient>(buffer_size);
		clients[i]->GetClient()->FakeUploadUniqueId(randGen);
	}

	f2cmap->RegisterFunction(ComposePostServiceClient::FuncType::UPLOAD_UNIQUE_ID, clients);
}

void InitializeFuncMapRedis(FunctionClientMap<FakeRedisClient> *f2cmap,
																RandomGenerator *randGen,
																int num_template_clients,
																int num_msg_per_client,
																int base_buffer_size) {

	uint64_t buffer_size = num_msg_per_client * base_buffer_size;

	MyThriftClient<FakeRedisClient>** clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetCreator(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_CREATOR, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetText(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_TEXT, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetMedia(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_MEDIA, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetPostId(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_POST_ID, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetPostType(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_POST_TYPE, clients);
	
	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetUrls(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_URLS, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHSetUserMentions(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HS_USER_MENTIONS, clients);


	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetCreator(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_CREATOR, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetText(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_TEXT, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetMedia(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_MEDIA, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetPostId(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_POST_ID, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetPostType(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_POST_TYPE, clients);
	
	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetUrls(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_URLS, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHGetUserMentions(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::HG_USER_MENTIONS, clients);

	clients = new MyThriftClient<FakeRedisClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRedisClient>(buffer_size);
		clients[i]->GetClient()->FakeHHIncrBy(randGen);
	}
	f2cmap->RegisterFunction(FakeRedisClient::FuncType::H_INC, clients);
}

void InitializeFuncMapPostStorage(FunctionClientMap<PostStorageServiceClient> *f2cmap,
																RandomGenerator *randGen,
																int num_template_clients,
																int num_msg_per_client,
																int base_buffer_size) {

	uint64_t buffer_size = num_msg_per_client * base_buffer_size;

	MyThriftClient<PostStorageServiceClient>** clients = new MyThriftClient<PostStorageServiceClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<PostStorageServiceClient>(buffer_size);
		clients[i]->GetClient()->FakeStorePost(randGen);
	}
	f2cmap->RegisterFunction(PostStorageServiceClient::FuncType::STORE_POST, clients);

}

void InitializeFuncMapUserTimeline(FunctionClientMap<UserTimelineServiceClient> *f2cmap,
																RandomGenerator *randGen,
																int num_template_clients,
																int num_msg_per_client,
																int base_buffer_size) {

	uint64_t buffer_size = num_msg_per_client * base_buffer_size;

	MyThriftClient<UserTimelineServiceClient>** clients = new MyThriftClient<UserTimelineServiceClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<UserTimelineServiceClient>(buffer_size);
		clients[i]->GetClient()->FakeWriteUserTimeline(randGen);
	}
	f2cmap->RegisterFunction(UserTimelineServiceClient::FuncType::WRITE_TIMELINE, clients);
}																

void InitializeFuncMapRabbitmq(FunctionClientMap<FakeRabbitmqClient> *f2cmap,
																RandomGenerator *randGen,
																int num_template_clients,
																int num_msg_per_client,
																int base_buffer_size) {

	uint64_t buffer_size = num_msg_per_client * base_buffer_size;

	MyThriftClient<FakeRabbitmqClient>** clients = new MyThriftClient<FakeRabbitmqClient>*[num_template_clients];
	// Fill up the clients
	for (int i = 0; i < num_template_clients; i++) {
		clients[i] = new MyThriftClient<FakeRabbitmqClient>(buffer_size);
		clients[i]->GetClient()->FakeUploadHomeTimeline(randGen);
	}
	f2cmap->RegisterFunction(FakeRabbitmqClient::FuncType::UPLOAD_TIMELINE, clients);

}