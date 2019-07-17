
# add_custom_command(OUTPUT gen-cpp/X gen-cpp/social_network_constants.cpp gen-cpp/social_network_types.cpp
#    COMMAND ${THRIFT_COMPILER}  -o ./ --gen cpp -r social_network.thrift
# )

set(thriftgencpp_SOURCES
    ${THRIFT_GEN_CPP_DIR}/MyUrlShortenService.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_constants.cpp
    ${THRIFT_GEN_CPP_DIR}/social_network_types.cpp
)

add_library(thriftgencpp STATIC ${thriftgencpp_SOURCES} ../Helpers/benchmarking.cpp)
target_link_libraries(thriftgencpp ${THRIFT_LIB})
target_include_directories(thriftgencpp PUBLIC
    ${THRIFT_INCLUDE_DIR}
)

#
#add_executable(MyUrlShortenServer
#        MyUrlShortenServer.cpp
#        ../Helpers/benchmarking.cpp ../Helpers/commons.h)

#target_link_libraries(MyUrlShortenServer
#    thriftgencpp
#    # ${THRIFT_LIB}
#    # ${CMAKE_THREAD_LIBS_INIT}
#)

add_executable(MyUrlShortenServer
        MyUrlShortenServer.cpp ${thriftgencpp_SOURCES}
        ../Helpers/benchmarking.cpp ../Helpers/commons.h)

target_link_libraries(MyUrlShortenServer
#        thriftgencpp
         ${THRIFT_LIB}
        # ${CMAKE_THREAD_LIBS_INIT}
)
target_include_directories(MyUrlShortenServer PUBLIC
        ${THRIFT_INCLUDE_DIR}
)

add_executable(MyUrlShortenClient MyUrlShortenClient.cpp ../Helpers/commons.h)
target_link_libraries(MyUrlShortenClient
    thriftgencpp
)

# install(TARGETS MyUrlShortenServer DESTINATION ./)