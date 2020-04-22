target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
        ${TELLO_INCLUDE}/command_factory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command_factory.cpp)

add_subdirectory(command)