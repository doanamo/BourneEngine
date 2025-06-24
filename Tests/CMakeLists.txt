cmake_minimum_required(VERSION 3.29)

project(Tests VERSION ${CMAKE_PROJECT_VERSION})

#
# Executable
#

add_executable(Tests
    "Testing/TestDefines.cpp"
    "Testing/TestRegistry.cpp"
    "Testing/TestObject.cpp"
    "Testing/TestGuards.cpp"
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

target_include_directories(Tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "../"
)

target_precompile_headers(Tests PRIVATE "Shared.hpp")

#
# Discovery
#

# fixme: Tests do not run on first fresh build, only on consecutive ones. This is an issue on build system as well.
# See: https://github.com/Kitware/CMake/blob/master/Modules/GoogleTest.cmake#L668
# See: https://github.com/Kitware/CMake/blob/ef28128d4b5f82d9a8a51acc378f2df309c6f240/Modules/GoogleTestAddTests.cmake#L116
get_target_property(TESTS_BINARY_DIR Tests BINARY_DIR)
file(TOUCH "${TESTS_BINARY_DIR}/DiscoveredTests.cmake")

add_custom_command(
    TARGET Tests POST_BUILD
    COMMENT "Discovering tests"
    BYPRODUCTS "${TESTS_BINARY_DIR}/DiscoveredTests.cmake"
    COMMAND Tests -WriteTests="${TESTS_BINARY_DIR}/DiscoveredTests.cmake"
)

include("${TESTS_BINARY_DIR}/DiscoveredTests.cmake")

#
# Dependencies
#

target_link_libraries(Tests PRIVATE Engine)
