
add_executable(
    SimpleService
    
    SimpleService.cpp
)

target_include_directories(
	SimpleService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
	SimpleService
    
    mycommon
		mysocialnet_types
		compose_post
		unique_id
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS SimpleService DESTINATION ./)