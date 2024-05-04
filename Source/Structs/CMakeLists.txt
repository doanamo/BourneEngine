cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Structs STATIC
    "Structs.cpp"
)

#
# Dependencies
#

target_link_libraries(Structs PUBLIC Common)
target_link_libraries(Structs PUBLIC Memory)
target_link_libraries(Structs PUBLIC Math)
