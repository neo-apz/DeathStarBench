
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

add_library(
    unique_id STATIC

    ${THRIFT_GEN_CPP_DIR}/MyUniqueIdService.cpp
)
target_include_directories(unique_id PUBLIC ${COMMON_DIR})