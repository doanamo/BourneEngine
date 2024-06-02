cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Containers STATIC
    "Containers.cpp"
)

#
# Dependencies
#

target_link_libraries(Containers PUBLIC Common)
target_link_libraries(Containers PUBLIC Memory)
target_link_libraries(Containers PUBLIC Math)
