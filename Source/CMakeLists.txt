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
    set_target_properties(Engine PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
    set_target_properties(Engine PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

#
# Modules
#

add_subdirectory(Common)
add_subdirectory(Logger)
add_subdirectory(Memory)
add_subdirectory(Math)
add_subdirectory(Structs)
add_subdirectory(Core)

#
# Dependencies
#

target_link_libraries(Engine PUBLIC Core)
