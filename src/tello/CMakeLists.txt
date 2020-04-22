target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
        ${TELLO_INCLUDE}/tello/command_factory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command_factory.cpp
        ${TELLO_INCLUDE}/tello/commander.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/commander.cpp
        ${TELLO_INCLUDE}/tello/response.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response_factory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response_factory.cpp)

add_subdirectory(command)
add_subdirectory(logger)