target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/response/status_response.hpp
        ${TELLO_INCLUDE}/tello/response/video_response.hpp
        ${TELLO_INCLUDE}/tello/response/query_response.hpp

        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/status_response.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/video_response.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/query_response.cpp)