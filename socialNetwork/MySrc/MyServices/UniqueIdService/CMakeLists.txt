
add_executable(
    UniqueIdService
    
    UniqueIdService.cpp
)

target_include_directories(
		UniqueIdService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
    UniqueIdService
    
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

install(TARGETS UniqueIdService DESTINATION ./)
