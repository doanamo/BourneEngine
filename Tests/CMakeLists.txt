cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_executable(Tests
    "Testing/TestObject.cpp"
    "Common/TestCommon.cpp"
    "Common/TestUtility.cpp"
    "Common/TestArray.cpp"
    "Common/TestString.cpp"
    "Common/TestStringView.cpp"
    "Common/TestUniquePtr.cpp"
    "Memory/TestMemory.cpp"
    "Memory/TestOperators.cpp"
    "Main.cpp"
)

setup_cmake_executable(Tests)

target_include_directories(Tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${PROJECT_SOURCE_DIR}"
)

target_precompile_headers(Tests PRIVATE "Shared.hpp")

#
# Dependencies
#

target_link_libraries(Tests PRIVATE Engine)
