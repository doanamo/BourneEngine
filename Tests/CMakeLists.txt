cmake_minimum_required(VERSION 3.29)

#
# Executable
#

enable_testing()
add_executable(Tests
    "Testing/TestObject.cpp"
    "Common/TestCommon.cpp"
    "Common/TestUtility.cpp"
    "Common/TestFunction.cpp"
    "Common/TestUniquePtr.cpp"
    "Common/TestArray.cpp"
    "Common/TestString.cpp"
    "Common/TestStringView.cpp"
    "Memory/TestMemory.cpp"
    "Memory/TestMemoryOperators.cpp"
    "Memory/TestMemoryStats.cpp"
    "Main.cpp"
)

setup_cmake_executable(Tests)
add_test(NAME "Tests" COMMAND Tests)

target_include_directories(Tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${PROJECT_SOURCE_DIR}"
)

target_precompile_headers(Tests PRIVATE "Shared.hpp")

#
# Dependencies
#

target_link_libraries(Tests PRIVATE Engine)
