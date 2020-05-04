if (WIN32)
    add_subdirectory(windows)
endif()

target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/native/network_interface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network_interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network_interface_factory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network_interface_factory.cpp)