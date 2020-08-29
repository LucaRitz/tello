target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/connection/tello_network.hpp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/udp_listener.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network_interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/network.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/udp_command_listener.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/udp_command_listener.cpp)