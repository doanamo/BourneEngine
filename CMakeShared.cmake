cmake_minimum_required(VERSION 3.28)

#
# Global
#

set(CMAKE_VERBOSE_MAKEFILE OFF)

#
# Options
#

option(VERBOSE "Enable verbose logging" OFF)
option(USE_ASAN "Enable address sanitizer" OFF)
option(USE_UBSAN "Enable undefined behavior sanitizer" OFF)

#
# Utility
#

function(print_variable variable)
    message(STATUS "${variable} = ${${variable}}")
endfunction()

function(set_cache variable value)
    string(STRIP "${value}" stripped)
    set(${variable} ${stripped} CACHE INTERNAL "")
endfunction()

function(remove_flag variable flags)
    string(FIND "${${variable}} " "${flags} " found)
    if(NOT found EQUAL -1) 
        string(REPLACE "${flags} " "" removed "${${variable}} ")
        string(REPLACE "  " " " removed "${removed}")
        set_cache(${variable} "${removed}")
    endif()
endfunction()

function(append_flag variable flags)
    remove_flag(${variable} "${flags}")
    set_cache(${variable} "${${variable}} ${flags}")
endfunction()

#
# Shared
#

# Enforces shared setup across all targets in this project.
# This is done because defaults in CMake are quite poor and cannot be always
# changed or overridden without modifying compiler/linker flags directly.
function(setup_cmake_shared)
    # Check supported platforms.
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_compile_definitions("PLATFORM_WINDOWS")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_compile_definitions("PLATFORM_LINUX")
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()
    
    # Check supported compilers.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_definitions("COMPILER_MSVC")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_definitions("COMPILER_CLANG")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_definitions("COMPILER_GCC")
    else()
        message(FATAL_ERROR "Unsupported compiler")
    endif()

    # Prevent this function from running more than once.
    if(CUSTOM_CMAKE_SETUP_CALLED)
        message(FATAL_ERROR "Custom CMake setup already called")
    endif()

    set(CUSTOM_CMAKE_SETUP_CALLED TRUE PARENT_SCOPE)

    # Enable unity build.
    set_cache(CMAKE_UNITY_BUILD ON)

    # Enable folders feature in generated Visual Studio solution.
    set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

    # Specify required C++ standard version.
    set_cache(CMAKE_CXX_STANDARD 20)
    set_cache(CMAKE_CXX_STANDARD_REQUIRED TRUE)

    # Treat all warnings as errors.
    set_cache(CMAKE_COMPILE_WARNING_AS_ERROR TRUE)

    # Define custom set of configurations.
    set_cache(CMAKE_CONFIGURATION_TYPES "Debug;Develop;Release")

    # Set initial flags for Debug and RelWithDebInfo configurations.
    # RelWithDebInfo is used as base for Develop and Release configurations.
    set_cache(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG_INIT}")
    set_cache(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG_INIT}")
    set_cache(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG_INIT}")
    set_cache(CMAKE_RC_FLAGS_DEBUG "${CMAKE_RC_FLAGS_DEBUG_INIT}")
    set_cache(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG_INIT}")
    set_cache(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT}")
    set_cache(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT}")

    set_cache(CMAKE_ASM_FLAGS_RELWITHDEBINFO "${CMAKE_ASM_FLAGS_RELWITHDEBINFO_INIT}")
    set_cache(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}")
    set_cache(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT}")
    set_cache(CMAKE_RC_FLAGS_RELWITHDEBINFO "${CMAKE_RC_FLAGS_RELWITHDEBINFO_INIT}")
    set_cache(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO_INIT}")
    set_cache(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO_INIT}")
    set_cache(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT}")

    # Disable suggestions to use safer standard library functions.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_definitions("-D_CRT_SECURE_NO_WARNINGS")
    endif()

    # Disable runtime type information.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_CXX_FLAGS "/GR-")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        append_flag(CMAKE_CXX_FLAGS "-fno-rtti")
    endif()

    # Disable exceptions.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        remove_flag(CMAKE_CXX_FLAGS "/EHsc")
        append_flag(CMAKE_CXX_FLAGS "/EHs-c-")
        add_compile_definitions("-D_HAS_EXCEPTIONS=0")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        append_flag(CMAKE_CXX_FLAGS "-fno-exceptions")
    endif()

    # Enable fast math by default.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_CXX_FLAGS "/fp:fast")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        append_flag(CMAKE_CXX_FLAGS "-ffast-math")
    endif()

    # Add global defines for identifying each configuration from code.
    add_compile_definitions(
        CONFIG_DEBUG=$<CONFIG:Debug>
        CONFIG_DEVELOP=$<CONFIG:Develop>
        CONFIG_RELEASE=$<CONFIG:Release>
    )

    # Make Release same as removed RelWithDebInfo configuration.
    set_cache(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_RC_FLAGS_RELEASE "${CMAKE_RC_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")

    # Make Develop behave like Release configuration (without additional changes below).
    set_cache(CMAKE_ASM_FLAGS_DEVELOP "${CMAKE_ASM_FLAGS_RELEASE}")
    set_cache(CMAKE_C_FLAGS_DEVELOP "${CMAKE_C_FLAGS_RELEASE}")
    set_cache(CMAKE_CXX_FLAGS_DEVELOP "${CMAKE_CXX_FLAGS_RELEASE}")
    set_cache(CMAKE_RC_FLAGS_DEVELOP "${CMAKE_RC_FLAGS_RELEASE}")
    set_cache(CMAKE_STATIC_LINKER_FLAGS_DEVELOP "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}")
    set_cache(CMAKE_SHARED_LINKER_FLAGS_DEVELOP "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
    set_cache(CMAKE_EXE_LINKER_FLAGS_DEVELOP "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

    # Select MSVC runtime library based on configuration.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set_cache(CMAKE_MSVC_RUNTIME_LIBRARY "")
        append_flag(CMAKE_C_FLAGS_DEBUG "/MDd")
        append_flag(CMAKE_CXX_FLAGS_DEBUG "/MDd")
        append_flag(CMAKE_C_FLAGS_DEVELOP "/MD")
        append_flag(CMAKE_CXX_FLAGS_DEVELOP "/MD")
        append_flag(CMAKE_C_FLAGS_RELEASE "/MD")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "/MD")
    endif()

    # Enable debug info for all configurations.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set_cache(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "")

        if(USE_ASAN OR USE_UBSAN)
            # Edit and continue cannot be used with sanitizers.
            append_flag(CMAKE_C_FLAGS "/Zi")
            append_flag(CMAKE_CXX_FLAGS "/Zi")
        else()
            # Enable debug info with support for hot reload in non-Release configurations.
            # Hot reload not enabled for Release due to optimization reasons.
            append_flag(CMAKE_C_FLAGS_DEBUG "/ZI")
            append_flag(CMAKE_CXX_FLAGS_DEBUG "/ZI")
            append_flag(CMAKE_C_FLAGS_DEVELOP "/ZI")
            append_flag(CMAKE_CXX_FLAGS_DEVELOP "/ZI")
            append_flag(CMAKE_C_FLAGS_RELEASE "/Zi")
            append_flag(CMAKE_CXX_FLAGS_RELEASE "/Zi")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        append_flag(CMAKE_C_FLAGS_DEBUG "-g")
        append_flag(CMAKE_CXX_FLAGS_DEBUG "-g")
        append_flag(CMAKE_C_FLAGS_DEVELOP "-g")
        append_flag(CMAKE_CXX_FLAGS_DEVELOP "-g")
        append_flag(CMAKE_C_FLAGS_RELEASE "-g")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "-g")
    endif()

    # Strip full path to PDB file in the executable to anonymize it.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/PDBALTPATH:%_PDB%")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/PDBALTPATH:%_PDB%")
    endif()

    # Allows the compiler to expand any function not explicitly marked for no inlining.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        remove_flag(CMAKE_C_FLAGS_RELEASE "/Ob1")
        remove_flag(CMAKE_CXX_FLAGS_RELEASE "/Ob1")
        append_flag(CMAKE_C_FLAGS_RELEASE "/Ob2")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "/Ob2")
    endif()

    # Explicitly replace some function calls with intrinsics.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_C_FLAGS_RELEASE "/Oi")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "/Oi")
    endif()

    # More aggressive optimizations in Release configuration.
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        remove_flag(CMAKE_C_FLAGS_RELEASE "-O2")
        remove_flag(CMAKE_CXX_FLAGS_RELEASE "-O2")
        append_flag(CMAKE_C_FLAGS_RELEASE "-O3")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()

    # Disable incremental linking for Release configuration and for sanitizers.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(USE_ASAN OR USE_UBSAN)
            remove_flag(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL")
            remove_flag(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL")
            remove_flag(CMAKE_SHARED_LINKER_FLAGS_DEVELOP "/INCREMENTAL")
            remove_flag(CMAKE_EXE_LINKER_FLAGS_DEVELOP "/INCREMENTAL")
            remove_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL")
            remove_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL")
            append_flag(CMAKE_SHARED_LINKER_FLAGS "/INCREMENTAL:NO")
            append_flag(CMAKE_EXE_LINKER_FLAGS "/INCREMENTAL:NO")
        else()
            remove_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL")
            remove_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL")
            append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
            append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
        endif()
    endif()

    # Enable link time optimizations in non-Debug configurations.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_C_FLAGS_RELEASE "/GL")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "/GL")
        append_flag(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/LTCG")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        append_flag(CMAKE_C_FLAGS_DEVELOP "-flto=thin")
        append_flag(CMAKE_CXX_FLAGS_DEVELOP "-flto=thin")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_DEVELOP "-flto=thin")
        append_flag(CMAKE_EXE_LINKER_FLAGS_DEVELOP "-flto=thin")
        append_flag(CMAKE_C_FLAGS_RELEASE "-flto=full")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "-flto=full")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-flto=full")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto=full")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        append_flag(CMAKE_C_FLAGS_RELEASE "-flto")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "-flto")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-flto")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")
    endif()

    # Removal of unused functions and sections in Release configuration.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/OPT:REF")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:REF")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/OPT:ICF")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:ICF")
    endif()

    # Enable compiler runtime sanitizers.
    if(USE_ASAN)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            append_flag(CMAKE_C_FLAGS "/fsanitize=address")
            append_flag(CMAKE_CXX_FLAGS "/fsanitize=address")
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
            append_flag(CMAKE_C_FLAGS "-fsanitize=address")
            append_flag(CMAKE_CXX_FLAGS "-fsanitize=address")
        endif()
    endif()

    if(USE_UBSAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
            append_flag(CMAKE_C_FLAGS_DEBUG "-fsanitize=undefined")
            append_flag(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined")
        endif()
    endif()

    # Do not omit frame pointers in Develop configuration.
    # Should result in better stack traces with optimizations enabled.
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        append_flag(CMAKE_C_FLAGS_DEVELOP "-fno-omit-frame-pointer")
        append_flag(CMAKE_CXX_FLAGS_DEVELOP "-fno-omit-frame-pointer")
    endif()

    # Compiler warning/error overrides.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92955
        append_flag(CMAKE_CXX_FLAGS "-Wno-stringop-overflow")
    endif()

    # Debug print of variables.
    if(VERBOSE)
        print_variable(CMAKE_C_COMPILER)
        print_variable(CMAKE_CXX_COMPILER)
        print_variable(CMAKE_LINKER)
        print_variable(CMAKE_AR)

        print_variable(CMAKE_UNITY_BUILD)
        print_variable(CMAKE_CXX_STANDARD)
        print_variable(CMAKE_CXX_STANDARD_REQUIRED)
        print_variable(CMAKE_COMPILE_WARNING_AS_ERROR)
        print_variable(CMAKE_CONFIGURATION_TYPES)

        print_variable(CMAKE_ASM_FLAGS)
        print_variable(CMAKE_C_FLAGS)
        print_variable(CMAKE_CXX_FLAGS)
        print_variable(CMAKE_RC_FLAGS)
        print_variable(CMAKE_STATIC_LINKER_FLAGS)
        print_variable(CMAKE_SHARED_LINKER_FLAGS)
        print_variable(CMAKE_EXE_LINKER_FLAGS)

        print_variable(CMAKE_ASM_FLAGS_DEBUG)
        print_variable(CMAKE_C_FLAGS_DEBUG)
        print_variable(CMAKE_CXX_FLAGS_DEBUG)
        print_variable(CMAKE_RC_FLAGS_DEBUG)
        print_variable(CMAKE_STATIC_LINKER_FLAGS_DEBUG)
        print_variable(CMAKE_SHARED_LINKER_FLAGS_DEBUG)
        print_variable(CMAKE_EXE_LINKER_FLAGS_DEBUG)

        print_variable(CMAKE_ASM_FLAGS_DEVELOP)
        print_variable(CMAKE_C_FLAGS_DEVELOP)
        print_variable(CMAKE_CXX_FLAGS_DEVELOP)
        print_variable(CMAKE_RC_FLAGS_DEVELOP)
        print_variable(CMAKE_STATIC_LINKER_FLAGS_DEVELOP)
        print_variable(CMAKE_SHARED_LINKER_FLAGS_DEVELOP)
        print_variable(CMAKE_EXE_LINKER_FLAGS_DEVELOP)

        print_variable(CMAKE_ASM_FLAGS_RELEASE)
        print_variable(CMAKE_C_FLAGS_RELEASE)
        print_variable(CMAKE_CXX_FLAGS_RELEASE)
        print_variable(CMAKE_RC_FLAGS_RELEASE)
        print_variable(CMAKE_STATIC_LINKER_FLAGS_RELEASE)
        print_variable(CMAKE_SHARED_LINKER_FLAGS_RELEASE)
        print_variable(CMAKE_EXE_LINKER_FLAGS_RELEASE)
    endif()
endfunction()

#
# Generated
#

find_package("Git")

function(setup_cmake_build_info target)
    # Build info is generated only once for final target.
    get_target_property(TARGET_BINARY_DIR ${target} BINARY_DIR)
    get_target_property(ENGINE_SOURCE_DIR Engine SOURCE_DIR)
    cmake_path(GET ENGINE_SOURCE_DIR PARENT_PATH ENGINE_ROOT_PATH)

    file(MAKE_DIRECTORY "${TARGET_BINARY_DIR}/Build")
    file(TOUCH "${TARGET_BINARY_DIR}/Build/Info.cpp")
    target_sources(${target} PRIVATE "${TARGET_BINARY_DIR}/Build/Info.cpp")

    add_custom_target(${target}BuildInfo
        COMMENT "Generating build info for ${target}"
        DEPENDS "${ENGINE_SOURCE_DIR}/Build/Info.cpp.in"
        BYPRODUCTS "${TARGET_BINARY_DIR}/Build/Info.cpp"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND}
            -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
            -D PROJECT_NAME=${CMAKE_PROJECT_NAME}
            -D PROJECT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/"
            -D ENGINE_SOURCE_PATH="${ENGINE_SOURCE_DIR}/"
            -D ENGINE_ROOT_PATH="${ENGINE_ROOT_PATH}/"
            -D INPUT_FILE="${ENGINE_SOURCE_DIR}/Build/Info.cpp.in"
            -D OUTPUT_FILE="${TARGET_BINARY_DIR}/Build/Info.cpp"
            -P "${ENGINE_SOURCE_DIR}/Build/Info.cmake"
    )

    add_dependencies(${target} ${target}BuildInfo)
endfunction()

function(setup_cmake_build_version target)
    # Engine and application have separate version files generated.
    if("${target}" STREQUAL "Engine")
        set(VERSION_PREFIX "Engine")
    else()
        set(VERSION_PREFIX "Application")
    endif()

    get_target_property(TARGET_BINARY_DIR ${target} BINARY_DIR)
    get_target_property(ENGINE_SOURCE_DIR Engine SOURCE_DIR)

    file(MAKE_DIRECTORY "${TARGET_BINARY_DIR}/Build")
    file(TOUCH "${TARGET_BINARY_DIR}/Build/Version.cpp")
    target_sources(${target} PRIVATE "${TARGET_BINARY_DIR}/Build/Version.cpp")

    add_custom_target(${target}BuildVersion
        COMMENT "Generating build version for ${target}"
        DEPENDS "${ENGINE_SOURCE_DIR}/Build/Version.cpp.in"
        BYPRODUCTS "${TARGET_BINARY_DIR}/Build/Version.cpp"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND}
            -D VERSION_PREFIX=${VERSION_PREFIX}
            -D PROJECT_VERSION=${PROJECT_VERSION}
            -D PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
            -D PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
            -D PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
            -D INPUT_FILE="${ENGINE_SOURCE_DIR}/Build/Version.cpp.in"
            -D OUTPUT_FILE="${TARGET_BINARY_DIR}/Build/Version.cpp"
            -P "${ENGINE_SOURCE_DIR}/Build/Version.cmake"
    )

    add_dependencies(${target} ${target}BuildVersion)
endfunction()

#
# Targets
#

function(setup_cmake_executable target)
    # Generate files needed for executable build.
    setup_cmake_build_info(${target})
    setup_cmake_build_version(${target})

    # Use main() instead of WinMain() on Windows.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set_target_properties(${target} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif()

    # Strip symbols in Release configuration on Linux.
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_custom_command(
            TARGET "${target}" POST_BUILD
            COMMAND $<$<CONFIG:Release>:${CMAKE_STRIP}>
            ARGS --strip-all "$<TARGET_FILE:${target}>" -o "$<TARGET_FILE:${target}>Stripped"
        )
    endif()
endfunction()
