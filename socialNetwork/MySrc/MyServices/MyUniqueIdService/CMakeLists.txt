
add_executable(
    MyUniqueIdService
    
    MyUniqueIdService.cpp
		${THRIFT_GEN_CPP_DIR}/MyUniqueIdService.cpp
		${THRIFT_GEN_CPP_DIR}/ComposePostService.cpp
)

target_include_directories(
		MyUniqueIdService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
    MyUniqueIdService
    
    mycommon
    mysocialnet_types
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS MyUniqueIdService DESTINATION ./)