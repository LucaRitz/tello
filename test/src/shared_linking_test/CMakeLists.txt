project(shared_linking_test)

# Setup testing
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(${PROJECT_NAME})
set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

enable_testing()

include(../../lib/googletest.cmake)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE tello gtest gtest_main gmock gmock_main)
