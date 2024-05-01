cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Common STATIC
    "Debug.cpp"
)

target_compile_definitions(Common PRIVATE COMMON_BUILD)

#
# Shared
#

target_include_directories(Common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../Include/")
