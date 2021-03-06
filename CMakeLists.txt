project(gtestexample)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(UNIX)
	set(BIN_OUTPUT_DIR ${PROJECT_SOURCE_DIR})
	set(LIB_OUTPUT_DIR ${PROJECT_SOURCE_DIR})
else()
	set(BIN_OUTPUT_DIR ${PROJECT_SOURCE_DIR})
	set(LIB_OUTPUT_DIR ${PROJECT_SOURCE_DIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_OUTPUT_DIR}")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_OUTPUT_DIR}")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BIN_OUTPUT_DIR}")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BIN_OUTPUT_DIR}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BIN_OUTPUT_DIR}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BIN_OUTPUT_DIR}")
endif()
	
file(GLOB_RECURSE MAIN_FILES ${PROJECT_SOURCE_DIR}/test_task.cpp)
source_group("Main" FILES ${MAIN_FILES})

include_directories(
	${GTEST_INCLUDE_DIR})

add_executable(gtestexample 
	${MAIN_FILES}
	)

#ifwindows	
#_DEBUG;_CONSOLE;_WIN32_WINDOWS;_WINSOCK_DEPRECATED_NO_WARNINGS;ASIO_HAS_SERIAL_PORT;ASIO_HAS_IOCP;
if(WIN32)
	message("OS is windows...")
	if(CMAKE_BUILD_TYPE MATCHES "debug")
		add_definitions(
		-D_DEBUG
		-D_CONSOLE
		-D_WIN32_WINDOWS
		-D_WINSOCK_DEPRECATED_NO_WARNINGS
		-DASIO_HAS_SERIAL_PORT
		-DASIO_HAS_IOCP)
	else()
		add_definitions(
		-DNDEBUG
		-D_CONSOLE
		-D_WIN32_WINDOWS
		-D_WINSOCK_DEPRECATED_NO_WARNINGS
		-DASIO_HAS_SERIAL_PORT
		-DASIO_HAS_IOCP)		
	endif()
endif()

#if(UNIX)
#	find_package(GTEST)
#	if (!GTEST_FOUND)
#    message(FATAL_ERROR "googletest library is needed but can't be found")
#	endif ()
	
#	link_directories(${GTEST_LIBRARY})
#	target_link_libraries(gtestexample gtest pthread)
#endif()

if(UNIX)
	target_link_libraries(gtestexample pthread)
endif()

set_target_properties(gtestexample PROPERTIES 
	OUTPUT_NAME_DEBUG "gtestexample"
	ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
