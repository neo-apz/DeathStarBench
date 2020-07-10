add_executable(
    TestUniqueIdService
    UniqueIdService.cpp
    ${THRIFT_GEN_CPP_DIR}/UniqueIdService.cpp
    ${THRIFT_GEN_CPP_DIR}/ComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)

add_executable(
    ReqGen
    RequestGen.cpp
    ${THRIFT_GEN_CPP_DIR}/UniqueIdService.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)

add_executable(
    FakeComposePost
    FakeComposePost.cpp
    ${THRIFT_GEN_CPP_DIR}/ComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)

add_executable(
    TestFunction
    TestFunction.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)


target_include_directories(
    TestUniqueIdService PRIVATE
)

target_include_directories(
    ReqGen PRIVATE
)

target_include_directories(
    FakeComposePost PRIVATE
)

target_include_directories(
    TestFunction PRIVATE
)


target_link_libraries(
    TestUniqueIdService
    nlohmann_json::nlohmann_json
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

target_link_libraries(
    ReqGen
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)

target_link_libraries(
    FakeComposePost
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)

target_link_libraries(
    TestFunction
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)


install(TARGETS TestUniqueIdService DESTINATION ./)