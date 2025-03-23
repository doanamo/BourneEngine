cmake_minimum_required(VERSION 3.29)

#
# Library
#

add_library(Engine STATIC
    "Common/Debug/Assert.cpp"
    "Common/Logger/Logger.cpp"
    "Common/Logger/Message.cpp"
    "Common/Logger/Format.cpp"
    "Memory/Stats.cpp"
    "Memory/Allocators/Default.cpp"
    "Platform/Memory.cpp"
    "Platform/Time.cpp"
    "Platform/CommandLine.cpp"
    "Platform/Window.cpp"
    "Platform/Utility.cpp"
    "Graphics/System.cpp"
    "Graphics/Stats.cpp"
    "Engine.cpp"
)

setup_cmake_build_version(Engine)

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
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if(TRUE)
    target_sources(Engine PRIVATE
        "Graphics/Vulkan/Allocator.cpp"
        "Graphics/Vulkan/Instance.cpp"
        "Graphics/Vulkan/System.cpp"
    )

    target_compile_definitions(Engine PUBLIC "GRAPHICS_VULKAN")
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
    target_link_libraries(Engine PRIVATE xcb)
endif()

find_package(Vulkan REQUIRED)
target_include_directories(Engine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Engine PRIVATE Vulkan::Vulkan)
