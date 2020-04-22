project(tello_test)

# Setup testing
set(CMAKE_CXX_STANDARD 17)
enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(lib/googletest-release-1.10.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

include_directories(${tello_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME})
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock gmock_main)
target_link_libraries(${PROJECT_NAME} tello)