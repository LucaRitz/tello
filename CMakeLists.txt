cmake_minimum_required(VERSION 3.15)
project(tello)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION 2.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(TELLO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(spdlog REQUIRED)

add_library(tello STATIC)
set_target_properties(tello PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1)

include_directories(${TELLO_INCLUDE})

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(tello PRIVATE spdlog::spdlog ws2_32)