cmake_minimum_required(VERSION 3.15)
project(tello)

set(CMAKE_CXX_STANDARD 17)

set(TELLO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(tello STATIC)
include_directories(${TELLO_INCLUDE})

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(tello)