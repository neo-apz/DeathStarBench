
add_executable(
    UniqueIdService
    
    UniqueIdService.cpp
)

target_include_directories(
		UniqueIdService PRIVATE ${COMMON_DIR}
)

target_link_libraries(
    UniqueIdService
    
    mycommon
		mysocialnet_types
		compose_post
		unique_id
    ${THRIFT_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    Boost::log
    Boost::log_setup
)

install(TARGETS UniqueIdService DESTINATION ./)

if(CEREBROS)

	add_executable(
		CerbUniqueIdService
		
		CerbUniqueIdService.cpp
	)

	target_include_directories(
		CerbUniqueIdService PRIVATE ${COMMON_DIR}
	)

	target_link_libraries(
		CerbUniqueIdService
		
		mycommon
		mysocialnet_types
		compose_post
		unique_id
		${THRIFT_LIB}
		${CMAKE_THREAD_LIBS_INIT}
		${Boost_LIBRARIES}
		Boost::log
		Boost::log_setup
	)

	install(TARGETS CerbUniqueIdService DESTINATION ./)

	endif()