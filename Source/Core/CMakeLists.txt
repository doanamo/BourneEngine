cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Core STATIC
    "Core.cpp"
    "CommandLine.cpp"
)

#
# Dependencies
#

target_link_libraries(Core PUBLIC Common)
target_link_libraries(Core PUBLIC Logger)
target_link_libraries(Core PUBLIC Memory)
