add_subdirectory(logger)
add_subdirectory(command)
add_subdirectory(connection)
add_subdirectory(response)
add_subdirectory(native)
add_subdirectory(thread)

target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/tello.hpp
        ${TELLO_INCLUDE}/tello/response.hpp
        ${TELLO_INCLUDE}/tello/swarm.hpp
        ${TELLO_INCLUDE}/tello/video_analyzer.hpp

        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tello.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/response.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/swarm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/video_analyzer.cpp)