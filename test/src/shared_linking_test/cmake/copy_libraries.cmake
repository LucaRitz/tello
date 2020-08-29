include(${CMAKE_SOURCE_DIR}/../../../../cmake/functions.cmake)

dynamic_lib_file_ending(DYNAMIC_LIB_FILE_ENDING)

# Remove all libs
file(GLOB CURRENT_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.${DYNAMIC_LIB_FILE_ENDING})
if (CURRENT_FILES)
    file(REMOVE ${CURRENT_FILES})
endif ()

# Copy tello libs
file(GLOB LIBRARY_FILES ${CMAKE_BINARY_DIR}/../../../*.${DYNAMIC_LIB_FILE_ENDING})
file(COPY ${LIBRARY_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})