project(tello_test)

# Setup testing
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

enable_testing()

include(lib/googletest.cmake)

add_executable(tello_test)
target_include_directories(tello_test PRIVATE ${tello_SOURCE_DIR}/src)

add_subdirectory(src)

target_link_libraries(tello_test PRIVATE tello gtest gtest_main gmock gmock_main)
