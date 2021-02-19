
add_executable(
	UserService

	MyUserService.cpp
)

target_include_directories(
	UserService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
	UserService
	
	user
	fake_memcached
	fake_mongo
	social_graph
	compose_post
	mycommon
	mysocialnet_types
	${THRIFT_LIB}
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	Boost::log
	Boost::log_setup
	OpenSSL::SSL
)

install(TARGETS UserService DESTINATION ./)