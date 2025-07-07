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
    "Common/TestStringShared.cpp"
    "Common/TestSorting.cpp"
    "Memory/TestAllocations.cpp"
    "Memory/TestAllocators.cpp"
    "Tests.cpp"
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

get_target_property(TESTS_BINARY_DIR Tests BINARY_DIR)
set(TEST_FILE "${TESTS_BINARY_DIR}/CTestTestfile.cmake")

add_custom_command(
    TARGET Tests POST_BUILD
    COMMENT "Discovering tests"
    BYPRODUCTS ${TEST_FILE}
    COMMAND Tests -DiscoverTests="${TEST_FILE}"
)

#
# Dependencies
#

target_link_libraries(Tests PRIVATE Engine)
