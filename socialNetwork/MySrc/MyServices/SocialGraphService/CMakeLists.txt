
add_executable(
    SocialGraphService

    SocialGraphService.cpp
)

target_include_directories(
    SocialGraphService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
    SocialGraphService
    
    social_graph
		user
		fake_mongo
		fake_redis
		mycommon
    mysocialnet_types
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS SocialGraphService DESTINATION ./)