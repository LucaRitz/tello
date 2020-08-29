cmake_minimum_required(VERSION 3.15)
project(tello)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION 2.0)

set(TELLO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(TELLO_BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(TELLO_MAKE_DLL "Make DLL" ON)

if (TELLO_BUILD_SHARED_LIBS)
    message(STATUS "Tello -- Build shared libs")
    add_library(tello SHARED)
else()
    message(STATUS "Tello -- Build static libs")
    add_library(tello STATIC)
endif ()

if (TELLO_MAKE_DLL)
    message(STATUS "Tello -- Export")
    target_compile_definitions(tello PUBLIC -DMAKE_DLL)
endif()

set_target_properties(tello PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

target_include_directories(tello
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(tello PUBLIC spdlog)
target_link_libraries(tello PRIVATE ws2_32 ctpl)
