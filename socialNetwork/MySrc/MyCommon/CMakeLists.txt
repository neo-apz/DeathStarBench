set(THRIFT_GEN_CPP_DIR "${PROJECT_SOURCE_DIR}/gen-cpp")
set(COMMON_DIR "${PROJECT_SOURCE_DIR}/MyCommon")
set(SON_COMMON_DIR "${COMMON_DIR}/son-common")
set(LIBSONUMA_DIR "${SON_COMMON_DIR}/libsonuma")
set(SON_TOOLS_DIR "${SON_COMMON_DIR}/tools")

set(LIBSON_SOURCES
		${LIBSONUMA_DIR}/sonuma.c
		${LIBSONUMA_DIR}/cer_rpc.cpp
    ${LIBSONUMA_DIR}/qflex_rmc_emu.c
)

set(TOOL_SRCS
    ${SON_TOOLS_DIR}/rd_pgmap.c
)

# Tools
add_library(son-tools STATIC ${TOOL_SRCS})
#Libsonuma
add_library(sonuma STATIC ${LIBSON_SOURCES})

target_link_libraries(sonuma ${LIBRARIES} son-tools)
target_compile_options(sonuma PUBLIC -D_GNU_SOURCE) # For sys/ipc.h
target_include_directories(sonuma PUBLIC ${LIBSONUMA_DIR})


add_library(
    mycommon STATIC

    ${COMMON_DIR}/MyTMemoryBuffer.cpp
    ${COMMON_DIR}/RandomGenerator.cpp
    ${COMMON_DIR}/core_schedule.cpp
		${COMMON_DIR}/MyProcessorEventHandler.cpp
)

target_link_libraries(mycommon sonuma)

add_library(
    mysocialnet_types STATIC

    ${THRIFT_GEN_CPP_DIR}/my_social_network_types.cpp
)
