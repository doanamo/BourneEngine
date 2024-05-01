cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Memory STATIC
    "DefaultAllocator.cpp"
)

#
# Dependencies
#

target_link_libraries(Memory PUBLIC Common)
