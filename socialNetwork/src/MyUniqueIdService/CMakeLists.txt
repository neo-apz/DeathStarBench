if (FLEXUS)
    add_definitions(-DFLEXUS)
    message("-- Flexus enabled!")
endif()

add_executable(
    MyUniqueIdService
    MyUniqueIdService.cpp
    ../MyCommon/MyTMemoryBuffer.cpp
    ../MyCommon/rd_pgmap.c
    ${THRIFT_GEN_CPP_DIR}/MyUniqueIdService.cpp
    ${THRIFT_GEN_CPP_DIR}/MyComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)

target_include_directories(
    MyUniqueIdService PRIVATE
)

target_link_libraries(
    MyUniqueIdService
    nlohmann_json::nlohmann_json
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS MyUniqueIdService DESTINATION ./)