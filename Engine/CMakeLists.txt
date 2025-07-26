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
    "Graphics/RenderApi.cpp"
    "Graphics/Stats.cpp"
    "ExitCodes.cpp"
    "Application.cpp"
    "Engine.cpp"
    "Main.cpp"
)

setup_cmake_build_version(Engine)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(GRAPHICS_API "Direct3D11")
    target_sources(Engine PRIVATE
        "Platform/Windows/Thread.cpp"
        "Platform/Windows/Memory.cpp"
        "Platform/Windows/Time.cpp"
        "Platform/Windows/Window.cpp"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(GRAPHICS_API "Null")
    target_sources(Engine PRIVATE
        "Platform/Linux/Thread.cpp"
        "Platform/Linux/Memory.cpp"
        "Platform/Linux/Time.cpp"
        "Platform/Null/Window.cpp"
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if(${GRAPHICS_API} STREQUAL "Direct3D11")
    target_compile_definitions(Engine PUBLIC "GRAPHICS_D3D11")
    target_link_libraries(Engine "d3d11.lib" "dxgi.lib" "d3dcompiler.lib" "dxguid.lib")
    target_sources(Engine PRIVATE
        "Graphics/Direct3D11/RenderApi.cpp"
    )
elseif(${GRAPHICS_API} STREQUAL "Null")
    target_compile_definitions(Engine PUBLIC "GRAPHICS_NULL")
    target_sources(Engine PRIVATE
        "Graphics/Null/RenderApi.cpp"
    )
else()
    message(FATAL_ERROR "Unspecified graphics API")
endif()

target_include_directories(Engine
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_precompile_headers(Engine PRIVATE "Shared.hpp")
