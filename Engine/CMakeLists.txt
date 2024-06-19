cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_library(Engine STATIC
    "Common/Debug/Debug.cpp"
    "Common/Debug/Assert.cpp"
    "Common/Platform/Thread.cpp"
    "Common/Platform/CommandLine.cpp"
    "Common/Logger/Logger.cpp"
    "Common/Logger/LoggerMessage.cpp"
    "Common/Logger/LoggerFormat.cpp"
    "Common/Logger/LoggerOutputDebugger.cpp"
    "Common/Logger/LoggerOutputConsole.cpp"
    "Memory/Memory.cpp"
    "Memory/OperatorNew.cpp"
    "Memory/DefaultAllocator.cpp"
    "Engine.cpp"
)

target_include_directories(Engine
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

#
# Version
#

find_package("Git")
add_custom_target(Version ALL
    COMMENT "Generating version header"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
    COMMAND ${CMAKE_COMMAND}
        -D GIT_EXECUTABLE="${GIT_EXECUTABLE}"
        -D PROJECT_VERSION="${PROJECT_VERSION}"
        -D PROJECT_VERSION_MAJOR="${PROJECT_VERSION_MAJOR}"
        -D PROJECT_VERSION_MINOR="${PROJECT_VERSION_MINOR}"
        -D PROJECT_VERSION_PATCH="${PROJECT_VERSION_PATCH}"
        -D INPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/Build/Version.hpp.in"
        -D OUTPUT_FILE="${CMAKE_CURRENT_BINARY_DIR}/Build/Version.hpp"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/Build/Version.cmake"
)

add_dependencies(Engine Version)
