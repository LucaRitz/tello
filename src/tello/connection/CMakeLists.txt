target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/connection/udp_listener.hpp
        ${TELLO_INCLUDE}/tello/connection/network.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network.cpp
        ${TELLO_INCLUDE}/tello/connection/udp_command_listener.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/udp_command_listener.cpp)