cmake_minimum_required(VERSION 3.5)
project(my_social_network_microservices)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_INSTALL_PREFIX /usr/local/bin)


include("../cmake/Findthrift.cmake")
include("../cmake/FindLibevent.cmake")

find_package(Threads)
find_package(OpenSSL REQUIRED)

set(Boost_USE_STATIC_LIBS ON)

if (ARM)
  message("-- Configuring for ARM.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a57+nosimd+nocrypto")
  find_package(Boost 1.58.0 EXACT REQUIRED COMPONENTS log log_setup)
else()
  message("-- Configuring for x86.")
  find_package(Boost 1.59.0 EXACT REQUIRED COMPONENTS log log_setup)
endif()

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif()


set(THRIFT_GEN_CPP_DIR ../../gen-cpp)
set(COMMON_DIR ../../MyCommon)

if (ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fPIE -pie -g")
    message("-- ASAN enabled!")
endif()

if (FLEXUS)
    add_definitions(-DFLEXUS)
    message("-- Flexus enabled!")
endif()

if (SW)
    add_definitions(-DSW)
    message("-- Stopwatch enabled!")
endif()

add_subdirectory(MyServices)

string( REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
