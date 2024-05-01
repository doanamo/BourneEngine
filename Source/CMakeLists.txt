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

target_compile_definitions(Engine PRIVATE ENGINE_BUILD)

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

add_subdirectory(Common)
target_link_libraries(Engine PUBLIC Common)

add_subdirectory(Memory)
target_link_libraries(Engine PUBLIC Memory)

add_subdirectory(Logger)
target_link_libraries(Engine PUBLIC Logger)

#
# Version
#

find_package("Git")

add_custom_target(Version
    COMMENT "Generating version header"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
    COMMAND ${CMAKE_COMMAND}
        -D GIT_EXECUTABLE="${GIT_EXECUTABLE}"
        -D PROJECT_VERSION="${PROJECT_VERSION}"
        -D PROJECT_VERSION_MAJOR="${PROJECT_VERSION_MAJOR}"
        -D PROJECT_VERSION_MINOR="${PROJECT_VERSION_MINOR}"
        -D PROJECT_VERSION_PATCH="${PROJECT_VERSION_PATCH}"
        -D INPUT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/Common/Version.hpp.in"
        -D OUTPUT_FILE="${CMAKE_CURRENT_BINARY_DIR}/Common/Version.hpp"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/Common/Version.cmake"
)

add_dependencies(Engine Version)
