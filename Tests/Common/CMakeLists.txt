cmake_minimum_required(VERSION 3.28)

#
# Executable
#

add_executable(TestCommon
    "TestCommon.cpp"
    "TestUtility.cpp"
)

setup_cmake_executable(TestCommon)

#
# Dependencies
#

target_link_libraries(TestCommon PUBLIC Common)
