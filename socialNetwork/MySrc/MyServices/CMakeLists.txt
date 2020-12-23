add_library(
    mycommon STATIC

    ${COMMON_DIR}/MyTMemoryBuffer.cpp
    ${COMMON_DIR}/RandomGenerator.cpp
    ${COMMON_DIR}/core_schedule.cpp
    ${COMMON_DIR}/MyProcessorEventHandler.cpp
)

add_library(
    mysocialnet_types STATIC

    ${THRIFT_GEN_CPP_DIR}/my_social_network_types.cpp
)

# My Services
add_subdirectory(MyUniqueIdService)
add_subdirectory(MyComposePostService)
add_subdirectory(MySocialGraphService)
add_subdirectory(MyUserService)

# Fake Services
# add_subdirectory(FakeComposePostService)