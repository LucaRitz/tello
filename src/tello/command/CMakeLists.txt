target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/command_command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/takeoff_command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/takeoff_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/land_command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/land_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/up_command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/up_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/streamon_command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/streamon_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/streamoff_command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/streamoff_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/wifi_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/validator_util.hpp
        )