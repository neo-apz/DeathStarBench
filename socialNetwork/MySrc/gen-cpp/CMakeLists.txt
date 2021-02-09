
add_library(
    mysocialnet_types STATIC

    ${THRIFT_GEN_CPP_DIR}/my_social_network_types.cpp
)
target_include_directories(mysocialnet_types PUBLIC ${COMMON_DIR})

add_library(
    compose_post STATIC

    ${THRIFT_GEN_CPP_DIR}/ComposePostService.cpp
)
target_include_directories(compose_post PUBLIC ${COMMON_DIR})
target_link_libraries(compose_post mysocialnet_types mycommon)

add_library(
    unique_id STATIC

    ${THRIFT_GEN_CPP_DIR}/UniqueIdService.cpp
)
target_include_directories(unique_id PUBLIC ${COMMON_DIR})
target_link_libraries(unique_id mysocialnet_types mycommon)

add_library(
    fake_redis STATIC

    ${THRIFT_GEN_CPP_DIR}/FakeRedis.cpp
)
target_include_directories(fake_redis PUBLIC ${COMMON_DIR})
target_link_libraries(fake_redis mysocialnet_types mycommon)

add_library(
    post_storage STATIC

    ${THRIFT_GEN_CPP_DIR}/PostStorageService.cpp
)
target_include_directories(post_storage PUBLIC ${COMMON_DIR})
target_link_libraries(post_storage mysocialnet_types mycommon)

add_library(
    user_timeline STATIC

    ${THRIFT_GEN_CPP_DIR}/UserTimelineService.cpp
)
target_include_directories(user_timeline PUBLIC ${COMMON_DIR})
target_link_libraries(user_timeline mysocialnet_types mycommon)

add_library(
    fake_rabbitmq STATIC

    ${THRIFT_GEN_CPP_DIR}/FakeRabbitmq.cpp
)
target_include_directories(fake_rabbitmq PUBLIC ${COMMON_DIR})
target_link_libraries(fake_rabbitmq mysocialnet_types mycommon)

add_library(
    user STATIC

    ${THRIFT_GEN_CPP_DIR}/UserService.cpp
)
target_include_directories(user PUBLIC ${COMMON_DIR})
target_link_libraries(user mysocialnet_types mycommon)

add_library(
    social_graph STATIC

    ${THRIFT_GEN_CPP_DIR}/MySocialGraphService.cpp
)
target_include_directories(social_graph PUBLIC ${COMMON_DIR})
target_link_libraries(social_graph mysocialnet_types mycommon)

add_library(
    fake_memcached STATIC

    ${THRIFT_GEN_CPP_DIR}/FakeMemcached.cpp
)
target_include_directories(fake_memcached PUBLIC ${COMMON_DIR})
target_link_libraries(fake_memcached mysocialnet_types mycommon)

add_library(
    fake_mongo STATIC

    ${THRIFT_GEN_CPP_DIR}/FakeMongo.cpp
)
target_include_directories(fake_mongo PUBLIC ${COMMON_DIR})
target_link_libraries(fake_mongo mysocialnet_types mycommon)