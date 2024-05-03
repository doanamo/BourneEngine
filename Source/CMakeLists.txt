cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_executable(Engine
    "Main.cpp"
)

target_include_directories(Engine
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

#
# Platforms
#

if(WIN32)
    set_target_properties(Engine PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(Engine PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

#
# Dependencies
#

add_subdirectory(Core)
target_link_libraries(Engine PUBLIC Core)
