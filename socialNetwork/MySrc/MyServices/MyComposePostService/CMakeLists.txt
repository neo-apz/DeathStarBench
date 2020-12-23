
add_executable(
    MyComposePostService

    MyComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/MyComposePostService.cpp
)

target_include_directories(
    MyComposePostService PRIVATE
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