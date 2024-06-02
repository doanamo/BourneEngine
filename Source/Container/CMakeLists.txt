cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Container STATIC
    "Container.cpp"
)

#
# Dependencies
#

target_link_libraries(Container PUBLIC Common)
target_link_libraries(Container PUBLIC Memory)
target_link_libraries(Container PUBLIC Math)
