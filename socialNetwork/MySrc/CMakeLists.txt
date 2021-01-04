cmake_minimum_required(VERSION 3.5)
project(my_social_network_microservices)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX /usr/local/bin)


include("cmake/Findthrift.cmake")
include("cmake/FindLibevent.cmake")

find_package(Threads)
find_package(OpenSSL REQUIRED)

set(Boost_USE_STATIC_LIBS ON)

if (ARM)
  message("-- Configuring for ARM.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a57+nosimd+nocrypto -DQFLEX")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a57+nosimd+nocrypto -DQFLEX")
  find_package(Boost 1.58.0 EXACT REQUIRED COMPONENTS log log_setup)
else()
  message("-- Configuring for x86.")
  find_package(Boost 1.59.0 EXACT REQUIRED COMPONENTS log log_setup)
endif()

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif()

if (ASAN)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fPIE -pie -g")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fPIE -pie -g")
    message("-- ASAN enabled!")
endif()

option(DEBUG_SONUMA "Enable all DLog prints in libsonuma")
option(DEBUG_MAGIC "Enable all magic instruction debug breakpoints.")

if(DEBUG_SONUMA)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -DDEBUG_PERF")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -DDEBUG_PERF")
	message("SoNUMA Debug is enabled!")
endif()

if(DEBUG_MAGIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPP_DEBUG")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAPP_DEBUG")
  message("APP_DEBUG is enabled!")
endif()

if (FLEXUS)
    add_definitions(-DFLEXUS)
    message("-- Flexus enabled!")
endif()

if (SW)
    add_definitions(-DSW)
    message("-- Stopwatch enabled!")
endif()

include(MyCommon/CMakeLists.txt)
add_subdirectory(MyServices)

string( REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
