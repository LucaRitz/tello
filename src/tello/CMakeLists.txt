add_subdirectory(logger)
add_subdirectory(command)
add_subdirectory(connection)
add_subdirectory(response)
add_subdirectory(native)
add_subdirectory(thread)

target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
        ${TELLO_INCLUDE}/tello/tello.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tello.cpp
        ${TELLO_INCLUDE}/tello/response.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response.cpp
        ${TELLO_INCLUDE}/tello/command_type.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command_type.cpp
        ${TELLO_INCLUDE}/tello/swarm.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/swarm.cpp
        ${TELLO_INCLUDE}/tello/video_analyzer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/video_analyzer.cpp)