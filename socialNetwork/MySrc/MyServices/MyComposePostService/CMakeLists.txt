
add_executable(
    MyComposePostService

    MyComposePostService.cpp
		${THRIFT_GEN_CPP_DIR}/ComposePostService.cpp
		${THRIFT_GEN_CPP_DIR}/FakeRedis.cpp
		${THRIFT_GEN_CPP_DIR}/PostStorageService.cpp
		${THRIFT_GEN_CPP_DIR}/UserTimelineService.cpp
)

target_include_directories(
    MyComposePostService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
    MyComposePostService

    mycommon
    mysocialnet_types
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS MyComposePostService DESTINATION ./)