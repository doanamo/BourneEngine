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
set(INCLUDE_TESTS_FILE "${TESTS_BINARY_DIR}/IncludeTests.cmake")
set(DISCOVERED_TESTS_FILE "${TESTS_BINARY_DIR}/DiscoveredTests.cmake")

add_custom_command(
    TARGET Tests POST_BUILD
    COMMENT "Discovering tests"
    BYPRODUCTS ${DISCOVERED_TESTS_FILE}
    COMMAND Tests -DiscoverTests="${DISCOVERED_TESTS_FILE}"
)

file(WRITE ${INCLUDE_TESTS_FILE}
    "if(EXISTS \"${DISCOVERED_TESTS_FILE}\")\n"
    "  include(\"${DISCOVERED_TESTS_FILE}\")\n"
    "else()\n"
    "  add_test(${TARGET}_NOT_BUILT ${TARGET}_NOT_BUILT)\n"
    "endif()\n"
)

set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES ${INCLUDE_TESTS_FILE})

#
# Dependencies
#

target_link_libraries(Tests PRIVATE Engine)
