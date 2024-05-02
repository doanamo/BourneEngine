cmake_minimum_required(VERSION 3.28)

#
# Library
#

add_library(Logger STATIC
    "Logger.cpp"
    "LoggerMessage.cpp"
    "LoggerFormat.cpp"
)

target_compile_definitions(Logger PRIVATE LOGGER_BUILD)

#
# Dependencies
#

target_link_libraries(Logger PUBLIC Common)
