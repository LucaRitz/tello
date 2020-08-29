project(shared_linking_test)

# Setup testing
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(${PROJECT_NAME})
add_custom_target(copy_shared_test_libraries COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_libraries.cmake)
add_dependencies(${PROJECT_NAME} copy_shared_test_libraries)

enable_testing()

include(../../lib/googletest.cmake)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE tello gtest gtest_main gmock gmock_main)
