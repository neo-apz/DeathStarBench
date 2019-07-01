add_executable(
    MyTextService
    MyTextService.cpp
    ${THRIFT_GEN_CPP_DIR}/MyTextService.cpp
    ${THRIFT_GEN_CPP_DIR}/ComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/UserMentionService.cpp
    ${THRIFT_GEN_CPP_DIR}/UrlShortenService.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)

target_include_directories(
    MyTextService PRIVATE
    # /usr/local/include/jaegertracing
)

target_link_libraries(
    MyTextService
    nlohmann_json::nlohmann_json
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
    # jaegertracing
)

install(TARGETS MyTextService DESTINATION ./)