target_sources(${PROJECT_NAME} PUBLIC
        ${TELLO_INCLUDE}/tello/thread/thread_pool.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool_impl.hpp)