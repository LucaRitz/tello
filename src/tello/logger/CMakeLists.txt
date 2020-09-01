target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/logger/logger_interface.hpp

        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/logger_interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/logger.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp)