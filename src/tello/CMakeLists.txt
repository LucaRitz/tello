add_subdirectory(logger)
add_subdirectory(command)
add_subdirectory(connection)
add_subdirectory(response)

target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
        ${TELLO_INCLUDE}/tello/command_factory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command_factory.cpp
        ${TELLO_INCLUDE}/tello/tello.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tello.cpp
        ${TELLO_INCLUDE}/tello/response.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response_factory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response_factory.cpp
        ${TELLO_INCLUDE}/tello/command_type.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command_type.cpp)