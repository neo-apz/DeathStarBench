
add_executable(
    MyUniqueIdService
    
    MyUniqueIdService.cpp
)

target_include_directories(
		MyUniqueIdService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
    MyUniqueIdService
    
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

install(TARGETS MyUniqueIdService DESTINATION ./)