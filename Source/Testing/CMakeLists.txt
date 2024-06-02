cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Testing STATIC
    "Testing.cpp"
)

#
# Dependencies
#

target_link_libraries(Testing PUBLIC Common)
target_link_libraries(Testing PUBLIC Logger)
