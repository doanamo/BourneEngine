cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_executable(Example
    "Main.cpp"
)

setup_cmake_executable(Example)

#
# Dependencies
#

target_link_libraries(Example PRIVATE Engine)
