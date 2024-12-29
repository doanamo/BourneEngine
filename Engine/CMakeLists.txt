cmake_minimum_required(VERSION 3.29)

#
# Library
#

add_library(Engine STATIC
    "Common/Debug/Assert.cpp"
    "Common/Logger/Logger.cpp"
    "Common/Logger/LoggerMessage.cpp"
    "Common/Logger/LoggerFormat.cpp"
    "Memory/MemoryStats.cpp"
    "Memory/Allocators/DefaultAllocator.cpp"
    "Platform/Memory.cpp"
    "Platform/Time.cpp"
    "Platform/CommandLine.cpp"
    "Platform/Window.cpp"
    "Graphics/GraphicsSystem.cpp"
    "Graphics/GraphicsStats.cpp"
    "Engine.cpp"
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(Engine PRIVATE
        "Platform/Windows/Debug.cpp"
        "Platform/Windows/Thread.cpp"
        "Platform/Windows/Memory.cpp"
        "Platform/Windows/Time.cpp"
        "Platform/Windows/Window.cpp"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(Engine PRIVATE
        "Platform/Linux/Debug.cpp"
        "Platform/Linux/Thread.cpp"
        "Platform/Linux/Memory.cpp"
        "Platform/Linux/Time.cpp"
        "Platform/Linux/Window.cpp"
    )

    set_property(SOURCE "Platform/Linux/Window.cpp" PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

target_include_directories(Engine
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_precompile_headers(Engine PRIVATE "Shared.hpp")

#
# Dependencies
#

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    target_include_directories(Engine PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(Engine PRIVATE ${X11_LIBRARIES})
endif()

find_package(Vulkan REQUIRED)
target_include_directories(Engine PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Engine PRIVATE Vulkan::Vulkan)

#
# Build
#

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Build")
file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/Build/Info.hpp")
file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/Build/Version.hpp")

add_custom_target(BuildInfo ALL
    COMMENT "Generating build info header"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/Build/Info.hpp"
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
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/Build/Version.hpp"
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
