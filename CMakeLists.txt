cmake_minimum_required(VERSION 3.28)

#
# Project
#

project(GameEngine
    VERSION 0.1.0
    DESCRIPTION "Custom game engine"
    HOMEPAGE_URL "https://github.com/doanamo/"
    LANGUAGES ASM C CXX
)

#
# Setup
#

include("CMakeShared.cmake")
shared_cmake_setup()

#
# Sources
#

add_subdirectory(Source)
