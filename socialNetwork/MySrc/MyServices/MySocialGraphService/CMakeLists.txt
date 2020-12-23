
add_executable(
    MySocialGraphService

    MySocialGraphService.cpp
    ${THRIFT_GEN_CPP_DIR}/MySocialGraphService.cpp
)

target_include_directories(
    MySocialGraphService PRIVATE
)

target_link_libraries(
    MySocialGraphService
    
    mycommon
    mysocialnet_types
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS MySocialGraphService DESTINATION ./)