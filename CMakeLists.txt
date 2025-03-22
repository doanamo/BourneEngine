cmake_minimum_required(VERSION 3.29)

#
# Project
#

project(BourneEngine
    VERSION 0.1.0
    DESCRIPTION "Custom game engine"
    HOMEPAGE_URL "https://github.com/doanamo/BourneEngine"
    LANGUAGES C CXX
)

#
# Setup
#

include("CMakeShared.cmake")
setup_cmake_shared()

#
# Sources
#

add_subdirectory(Engine)
add_subdirectory(Plugins)
add_subdirectory(Example)

#
# Tests
#

enable_testing()
add_subdirectory(Tests)
