cmake_minimum_required(VERSION 3.29)

#
# Library
#

add_library(Engine STATIC
    "Common/Standard.cpp"
    "Common/Debug/Assert.cpp"
    "Memory/Memory.cpp"
    "Memory/MemoryStats.cpp"
    "Memory/Allocators/DefaultAllocator.cpp"
    "Memory/Allocators/InlineAllocator.cpp"
    "Platform/CommandLine.cpp"
    "Platform/Timer.cpp"
    "Platform/Window.cpp"
    "Graphics/GraphicsSystem.cpp"
    "Graphics/GraphicsStats.cpp"
    "Engine.cpp"
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    target_sources(Engine PRIVATE
        "Common/Logger/Logger.cpp"
        "Common/Logger/LoggerMessage.cpp"
        "Common/Logger/LoggerFormat.cpp"
    )

    target_compile_definitions(Engine PUBLIC "ENABLE_LOGGER")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(Engine PRIVATE
        "Platform/Windows/Debug.cpp"
        "Platform/Windows/Thread.cpp"
        "Platform/Windows/Timer.cpp"
        "Platform/Windows/Window.cpp"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(Engine PRIVATE
        "Platform/Linux/Debug.cpp"
        "Platform/Linux/Thread.cpp"
        "Platform/Linux/Timer.cpp"
        "Platform/Linux/Window.cpp"
    )
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

target_include_directories(Engine
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_precompile_headers(Engine PRIVATE "Shared.hpp")

#
# Build
#

add_custom_target(BuildInfo ALL
    COMMENT "Generating build info header"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
    COMMAND ${CMAKE_COMMAND}
        -D PROJECT_PATH="${PROJECT_SOURCE_DIR}/"
        -D INPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/Build/Info.hpp.in"
        -D OUTPUT_FILE="${CMAKE_CURRENT_BINARY_DIR}/Build/Info.hpp"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/Build/Info.cmake"
    }
)

find_package("Git")
add_custom_target(BuildVersion ALL
    COMMENT "Generating build version header"
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

add_dependencies(Engine BuildInfo BuildVersion)
