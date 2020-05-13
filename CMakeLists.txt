cmake_minimum_required(VERSION 3.15)
project(tello)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION 2.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(TELLO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(spdlog REQUIRED)

add_library(tello STATIC)

# First for the generic no-config case (e.g. with mingw)
set_target_properties(tello PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	set_target_properties(tello PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_BINARY_DIR}/lib)
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set_target_properties(tello PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

include_directories(${TELLO_INCLUDE})

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(tello spdlog_lib ws2_32)