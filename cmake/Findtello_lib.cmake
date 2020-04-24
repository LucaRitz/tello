add_library(tello_lib STATIC IMPORTED)
set_target_properties(tello_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${tello_SOURCE_DIR}/include"
        IMPORTED_LOCATION "${tello_BINARY_DIR}/lib/tello.lib"
        )

add_library(spdlog_lib STATIC IMPORTED)
set(spdlog_dir ${tello_SOURCE_DIR}/lib/spdlog)
set_target_properties(spdlog_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${spdlog_dir}/include"
        IMPORTED_LOCATION "${spdlog_dir}/spdlog.lib"
        )

set(TELLO_LIBS tello_lib spdlog_lib ws2_32)