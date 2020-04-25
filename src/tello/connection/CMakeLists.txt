target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/connection/connection_data.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/connection_data.cpp
        ${TELLO_INCLUDE}/tello/connection/udp_listener.hpp)