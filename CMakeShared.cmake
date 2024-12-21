cmake_minimum_required(VERSION 3.28)

#
# Global
#

set(CMAKE_VERBOSE_MAKEFILE OFF)

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
        message(FATAL_ERROR "Unsupported platform!")
    endif()
    
    # Check supported compilers.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_definitions("COMPILER_MSVC")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_definitions("COMPILER_CLANG")
    else()
        message(FATAL_ERROR "Unsupported compiler!")
    endif()

    # Prevent this function from running more than once.
    if(CUSTOM_CMAKE_SETUP_CALLED)
        message(FATAL_ERROR "Custom CMake setup already called!")
    endif()

    set(CUSTOM_CMAKE_SETUP_CALLED TRUE PARENT_SCOPE)

    # Enable unity build.
    #set_cache(CMAKE_UNITY_BUILD ON)

    # Enable folders feature in generated Visual Studio solution.
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

    # Specify required C++ standard version.
    set_cache(CMAKE_CXX_STANDARD 20)
    set_cache(CMAKE_CXX_STANDARD_REQUIRED ON)

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

    # Disable RTTI and exceptions.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        remove_flag(CMAKE_CXX_FLAGS "/EHsc")
        append_flag(CMAKE_CXX_FLAGS "/EHs-c-")
        append_flag(CMAKE_CXX_FLAGS "/GR-")
        add_compile_definitions("-D_HAS_EXCEPTIONS=0")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        append_flag(CMAKE_CXX_FLAGS "-fno-exceptions")
    endif()

    # Enable fast math by default.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_CXX_FLAGS "/fp:fast")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        append_flag(CMAKE_CXX_FLAGS "-ffast-math")
    endif()

    # Add global defines for identifying each configuration from code.
    add_compile_definitions(
        $<$<CONFIG:Debug>:CONFIG_DEBUG>
        $<$<CONFIG:Develop>:CONFIG_DEVELOP>
        $<$<CONFIG:Release>:CONFIG_RELEASE>
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
        # Enable debug info with support for hot reload in Develop configuration only.
        # Hot reload not enabled for debug due to incompatibility with ASAN.
        # Hot reload not enabled for Release due to optimization reasons.
        set_cache(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "")
        append_flag(CMAKE_C_FLAGS_DEBUG "/Zi")
        append_flag(CMAKE_CXX_FLAGS_DEBUG "/Zi")
        append_flag(CMAKE_C_FLAGS_DEVELOP "/ZI")
        append_flag(CMAKE_CXX_FLAGS_DEVELOP "/ZI")
        append_flag(CMAKE_C_FLAGS_RELEASE "/Zi")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "/Zi")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
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

    # More aggressive optimizations in Release configuration.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        remove_flag(CMAKE_C_FLAGS_RELEASE "-O2")
        remove_flag(CMAKE_CXX_FLAGS_RELEASE "-O2")
        append_flag(CMAKE_C_FLAGS_RELEASE "-O3")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()

    # Disable incremental linking for Release and Debug configuration.
    # Incremental linking is not supported with ASAN on Debug configuration.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        remove_flag(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL")
        remove_flag(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL")
        remove_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        remove_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO")
        append_flag(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
    endif()

    # Enable link time optimizations in Release configuration.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_C_FLAGS_RELEASE "/GL")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "/GL")
        append_flag(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
        append_flag(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/LTCG")
        append_flag(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        append_flag(CMAKE_C_FLAGS_RELEASE "-flto")
        append_flag(CMAKE_CXX_FLAGS_RELEASE "-flto")
        append_flag(CMAKE_STATIC_LINKER_FLAGS_RELEASE "-flto")
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

    # Enable ASAN for Debug configuration.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        append_flag(CMAKE_C_FLAGS_DEBUG "/fsanitize=address")
        append_flag(CMAKE_CXX_FLAGS_DEBUG "/fsanitize=address")
        append_flag(CMAKE_C_FLAGS_DEBUG "/fno-sanitize-address-vcasan-lib")
        append_flag(CMAKE_CXX_FLAGS_DEBUG "/fno-sanitize-address-vcasan-lib")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        append_flag(CMAKE_C_FLAGS_DEBUG "-fsanitize=address")
        append_flag(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address")
        append_flag(CMAKE_C_FLAGS_DEBUG "-fsanitize=undefined")
        append_flag(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined")
    endif()

    # Debug print of variables.
    if(FALSE)
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
# Targets
#

function(setup_cmake_executable target)
    # Custom memory operators need to be defined in every module for them to function properly.
    target_sources(${target} PRIVATE "../Engine/Memory/MemoryOperators.cpp")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set_target_properties(${target} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if(${target} STREQUAL "Tests")
            set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE FALSE)
        else()
            set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
        endif()
    endif()
endfunction()
