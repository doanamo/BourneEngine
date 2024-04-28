cmake_minimum_required(VERSION 3.28)

project(GameEngine
    VERSION 0.1.0
    DESCRIPTION "Custom game engine"
    HOMEPAGE_URL "https://github.com/doanamo/"
    LANGUAGES ASM C CXX
)

include("CMakeShared.cmake")
shared_cmake_setup()

add_subdirectory(Source)
