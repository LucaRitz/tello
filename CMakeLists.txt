cmake_minimum_required(VERSION 3.15)
project(tello)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION 2.0)

set(TELLO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(lib/spdlog.cmake)

add_library(tello STATIC)
set_target_properties(tello PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

target_include_directories(tello
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/ctpl)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(tello PUBLIC spdlog)
target_link_libraries(tello PRIVATE ws2_32)
