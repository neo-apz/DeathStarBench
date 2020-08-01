
add_executable(
    MyComposePostService
    MyComposePostService.cpp
    ${COMMON_DIR}/MyTMemoryBuffer.cpp
    ${THRIFT_GEN_CPP_DIR}/MyComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/FakeComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/my_social_network_types.cpp
)

target_include_directories(
    MyComposePostService PRIVATE
)

target_link_libraries(
    MyComposePostService
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS MyComposePostService DESTINATION ./)