
add_executable(
    MyUserService

    MyUserService.cpp
    
    ${COMMON_DIR}/MyTMemoryBuffer.cpp
    ${COMMON_DIR}/RandomGenerator.cpp
    ${COMMON_DIR}/core_schedule.cpp
    ${COMMON_DIR}/MyProcessorEventHandler.cpp
    
    ${THRIFT_GEN_CPP_DIR}/MyUserService.cpp
    ${THRIFT_GEN_CPP_DIR}/my_social_network_types.cpp
)

target_include_directories(
    MyUserService PRIVATE
)

target_link_libraries(
    MyUserService
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS MyUserService DESTINATION ./)