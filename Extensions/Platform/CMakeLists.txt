cmake_minimum_required(VERSION 3.29)

#
# Library
#

add_library(Platform STATIC "Platform.cpp")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(Platform PRIVATE
        "Windows/Debug.cpp"
        "Windows/Thread.cpp"
        "Windows/Memory.cpp"
        "Windows/Time.cpp"
        "Windows/Window.cpp"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(Platform PRIVATE
        "Linux/Debug.cpp"
        "Linux/Thread.cpp"
        "Linux/Memory.cpp"
        "Linux/Time.cpp"
        "Linux/Window.cpp"
    )

    set_property(SOURCE "Linux/Window.cpp" PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

target_include_directories(Platform
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${PROJECT_SOURCE_DIR}"
)

target_precompile_headers(Platform PRIVATE "Shared.hpp")

#
# Dependencies
#

target_link_libraries(Platform Engine)
