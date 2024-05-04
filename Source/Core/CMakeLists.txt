cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Core STATIC
    "Core.cpp"
    "CommandLine.cpp"
)

#
# Dependencies
#

add_subdirectory("../Common/" "../Common")
target_link_libraries(Core PUBLIC Common)

add_subdirectory("../Logger/" "../Logger")
target_link_libraries(Core PUBLIC Logger)

add_subdirectory("../Memory/" "../Memory")
target_link_libraries(Core PUBLIC Memory)
