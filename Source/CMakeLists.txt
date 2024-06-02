cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_executable(Engine
    "Main.cpp"
)

setup_cmake_executable(Engine)

target_include_directories(Engine
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

#
# Modules
#

add_subdirectory(Common)
add_subdirectory(Logger)
add_subdirectory(Memory)
add_subdirectory(Math)
add_subdirectory(Containers)
add_subdirectory(Core)
add_subdirectory(Testing)

#
# Dependencies
#

target_link_libraries(Engine PUBLIC Core)
