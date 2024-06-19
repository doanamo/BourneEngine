cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_executable(Tests
    "Main.cpp"
)

setup_cmake_executable(Tests)

#
# Dependencies
#

target_link_libraries(Tests PRIVATE Engine)
