
add_executable(
    SimpleUniqueIdService
    SimpleUniqueIdService.cpp
    ${COMMON_DIR}/MyTMemoryBuffer.cpp
    ${COMMON_DIR}/RandomGenerator.cpp
    ${COMMON_DIR}/core_schedule.cpp
    ${THRIFT_GEN_CPP_DIR}/MyUniqueIdService.cpp
    ${THRIFT_GEN_CPP_DIR}/my_social_network_types.cpp
)

target_include_directories(
    SimpleUniqueIdService PRIVATE
)

target_link_libraries(
    SimpleUniqueIdService
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS SimpleUniqueIdService DESTINATION ./)