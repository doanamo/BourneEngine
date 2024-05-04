cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Math STATIC
    "Math.cpp"
)

#
# Dependencies
#

target_link_libraries(Math PUBLIC Common)
