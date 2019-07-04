
# add_custom_command(OUTPUT gen-cpp/X gen-cpp/social_network_constants.cpp gen-cpp/social_network_types.cpp
#    COMMAND ${THRIFT_COMPILER}  -o ./ --gen cpp -r social_network.thrift
# )


add_executable(
    MyUrlShortenServer
    MyUrlShortenServer.cpp
    ${THRIFT_GEN_CPP_DIR}/MyUrlShortenService.cpp
    # ${THRIFT_GEN_CPP_DIR}/ComposePostService.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)

target_include_directories(
    MyUrlShortenServer PRIVATE
    # ${LIBMEMCACHED_INCLUDE_DIR}
    # ${MONGOC_INCLUDE_DIRS}
    # /usr/local/include/jaegertracing
)

target_link_libraries(
    MyUrlShortenServer
    # nlohmann_json::nlohmann_json
    # ${MONGOC_LIBRARIES}
    # ${LIBMEMCACHED_LIBRARIES}
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    # ${Boost_LIBRARIES}
    # Boost::log
    # Boost::log_setup
    # jaegertracing
)

# install(TARGETS MyUrlShortenServer DESTINATION ./)