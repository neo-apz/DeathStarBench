
add_executable(
    ComposePostService

    ComposePostService.cpp
)

target_include_directories(
    ComposePostService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
    ComposePostService

		compose_post
		user_timeline
		post_storage
		fake_redis
		mycommon
    mysocialnet_types
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS ComposePostService DESTINATION ./)