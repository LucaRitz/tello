add_library(spdlog_lib STATIC IMPORTED)
set(spdlog_dir ${tello_SOURCE_DIR}/lib/spdlog)
set_target_properties(spdlog_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${spdlog_dir}/include"
        IMPORTED_LOCATION "${spdlog_dir}/spdlog.lib"
        )