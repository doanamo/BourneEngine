cmake_minimum_required(VERSION 3.29)

#
# Executable
#

add_executable(Example
    "Main.cpp"
)

setup_cmake_executable(Example)
set_target_properties(Example PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

target_include_directories(Example
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${PROJECT_SOURCE_DIR}"
)

target_precompile_headers(Example PRIVATE "Shared.hpp")

#
# Dependencies
#

target_link_libraries(Example PRIVATE Engine)
