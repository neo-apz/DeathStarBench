cmake_minimum_required(VERSION 3.1)

project(soccialnet_myservices)

include("Findthrift.cmake")

set(THRIFT_ROOT /Users/neo/Desktop/thrift/thrift-playground/dl/thrift-0.12.0)
set(THRIFT_HOME ${THRIFT_ROOT})

#include("../cmake/Findthrift.cmake")
#include("./ThriftMacros.cmake")


set(THRIFT_INCLUDE_DIR "${THRIFT_ROOT}/lib/cpp/src/")
set(THRIFT_LIB "${THRIFT_ROOT}/cmake_build/lib/libthriftd.dylib")

include_directories("${THRIFT_ROOT}/lib/cpp/src/")

#add_library("/Users/neo/Desktop/thrift/thrift-playground/dl/thrift-0.12.0/cmake_build/lib/libthrift.dylib" STATIC)

find_package(Threads)

 set(Boost_USE_STATIC_LIBS ON)
 find_package(Boost 1.54.0 REQUIRED COMPONENTS log log_setup)
 if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
   link_directories(${Boost_LIBRARY_DIRS})
 endif()

set(THRIFT_GEN_CPP_DIR ../gen-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-O3")
# set(CMAKE_INSTALL_PREFIX /usr/local/bin)

option(FLEXUS "With Flexus" OFF)

if(FLEXUS)
  add_definitions(-DFLEXUS)
  set(CMAKE_CXX_FLAGS "-mcpu=cortex-a57+nosimd")
  message(STATUS "FLEXUS Enabled!")
endif()

add_subdirectory(MyUrlShortenService)



message(${THRIFT_LIB})
