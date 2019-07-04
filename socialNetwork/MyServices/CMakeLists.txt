cmake_minimum_required(VERSION 3.1)

project(soccialnet_myservices)

include("../cmake/Findthrift.cmake")

find_package(Threads)

# set(Boost_USE_STATIC_LIBS ON)
# find_package(Boost 1.54.0 REQUIRED COMPONENTS log log_setup)
# if(Boost_FOUND)
#   include_directories(${Boost_INCLUDE_DIRS})
#   link_directories(${Boost_LIBRARY_DIRS})
# endif()

set(THRIFT_GEN_CPP_DIR ../gen-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-O3")
# set(CMAKE_INSTALL_PREFIX /usr/local/bin)

add_subdirectory(MyUrlShortenService)
