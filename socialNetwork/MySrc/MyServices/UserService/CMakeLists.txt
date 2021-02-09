
add_executable(
    MyUserService

    MyUserService.cpp
    ${THRIFT_GEN_CPP_DIR}/MyUserService.cpp
)

target_include_directories(
    MyUserService PRIVATE
)

target_link_libraries(
    MyUserService
    
    mycommon
    mysocialnet_types
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS MyUserService DESTINATION ./)