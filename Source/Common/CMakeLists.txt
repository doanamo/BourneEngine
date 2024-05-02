cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Common STATIC
    "Debug.cpp"
    "Assert.cpp"
    "Thread.cpp"
)

target_compile_definitions(Common PRIVATE COMMON_BUILD)

#
# Shared
#

target_include_directories(Common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../Include/")

#
# Version
#

find_package("Git")

add_custom_target(Version ALL
    COMMENT "Generating version header"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../"
    COMMAND ${CMAKE_COMMAND}
        -D GIT_EXECUTABLE="${GIT_EXECUTABLE}"
        -D PROJECT_VERSION="${PROJECT_VERSION}"
        -D PROJECT_VERSION_MAJOR="${PROJECT_VERSION_MAJOR}"
        -D PROJECT_VERSION_MINOR="${PROJECT_VERSION_MINOR}"
        -D PROJECT_VERSION_PATCH="${PROJECT_VERSION_PATCH}"
        -D INPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/Version.hpp.in"
        -D OUTPUT_FILE="${CMAKE_CURRENT_BINARY_DIR}/Version.hpp"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/Version.cmake"
)

add_dependencies(Common Version)
