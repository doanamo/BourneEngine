cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_library(Engine STATIC
    "Common/Debug/Debug.cpp"
    "Common/Debug/Assert.cpp"
    "Memory/Operators.cpp"
    "Memory/DefaultAllocator.cpp"
    "Platform/CommandLine.cpp"
    "Engine.cpp"
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    target_sources(Engine PRIVATE
        "Common/Logger/Logger.cpp"
        "Common/Logger/LoggerMessage.cpp"
        "Common/Logger/LoggerFormat.cpp"
        "Common/Logger/LoggerOutputDebugger.cpp"
        "Common/Logger/LoggerOutputConsole.cpp"
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(Engine PRIVATE
        "Platform/Windows/Thread.cpp"
        "Platform/Windows/Window.cpp"
        "Graphics/Direct3D12/Context.cpp"
    )

    target_link_libraries(Engine "d3d12.lib" "dxgi.lib" "d3dcompiler.lib" "dxguid.lib") 
endif()

target_include_directories(Engine
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_precompile_headers(Engine PRIVATE "Shared.hpp")

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
