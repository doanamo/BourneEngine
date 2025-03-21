cmake_minimum_required(VERSION 3.29)

project(Tests VERSION ${CMAKE_PROJECT_VERSION})

#
# Executable
#

enable_testing()
add_executable(Tests
    "Testing/TestRegistry.cpp"
    "Testing/TestObject.cpp"
    "Testing/MemoryStats.cpp"
    "Common/TestUtility.cpp"
    "Common/TestScopeGuard.cpp"
    "Common/TestScopeValue.cpp"
    "Common/TestResult.cpp"
    "Common/TestOptional.cpp"
    "Common/TestFunction.cpp"
    "Common/TestUniquePtr.cpp"
    "Common/TestArray.cpp"
    "Common/TestString.cpp"
    "Common/TestStringView.cpp"
    "Memory/TestAllocations.cpp"
    "Memory/TestAllocators.cpp"
    "Main.cpp"
)

setup_cmake_executable(Tests)
add_test(NAME "Tests" COMMAND Tests)

target_include_directories(Tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "../"
)

target_precompile_headers(Tests PRIVATE "Shared.hpp")

#
# Dependencies
#

target_link_libraries(Tests PRIVATE Engine)
