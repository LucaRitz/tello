set(tello_lib_location ${tello_BINARY_DIR}/lib/tello.lib)
#if(EXISTS "${tello_BINARY_DIR}/lib/Release/tello.lib")
#    set(tello_lib_location ${tello_BINARY_DIR}/lib/Release/tello.lib)
#endif()

message(" *** BERGER TEST *** ")
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "BERGER --- ${_variableName}=${${_variableName}}")
#endforeach()

message("-- Use tello-lib-location: ${tello_lib_location}")

add_library(tello_lib STATIC IMPORTED)
set_target_properties(tello_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${tello_SOURCE_DIR}/include"
        IMPORTED_LOCATION ${tello_lib_location}
        )

set(TELLO_INCLUDE ${spdlog_lib}/include
        ${tello_SOURCE_DIR}/include)

set(TELLO_LIBS tello_lib spdlog_lib ws2_32)