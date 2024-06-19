cmake_minimum_required(VERSION 3.28)

#
# Cache
#

set(CURRENT_CACHE_VERSION 2)

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

function(remove_flags variable flags)
    string(FIND "${${variable}}" "${flags}" found)
    if(NOT found EQUAL -1) 
        string(REPLACE "${flags}" "" removed "${${variable}}")
        string(REPLACE "  " " " removed "${removed}")
        set_cache(${variable} "${removed}")
    endif()
endfunction()

function(append_flags variable flags)
    remove_flags(${variable} "${flags}")
    set_cache(${variable} "${${variable}} ${flags}")
endfunction()

function(prepend_flags variable flags)
    remove_flags(${variable} "${flags}")
    set_cache(${variable} "${flags} ${${variable}}")
endfunction()

#
# Shared
#

function(setup_cmake_shared)
    # Prevent this function from running more than once.
    if(CUSTOM_CMAKE_SETUP_CALLED)
        message(FATAL_ERROR "Custom CMake setup already called!")
    endif()

    set(CUSTOM_CMAKE_SETUP_CALLED TRUE PARENT_SCOPE)

    # Version cache in case of breaking changes that require cache reset.
    if(NOT CACHE_VERSION)
        set_cache(CACHE_VERSION ${CURRENT_CACHE_VERSION})
    elseif(NOT CACHE_VERSION EQUAL ${CURRENT_CACHE_VERSION})
        message(FATAL_ERROR "Cache version mismatch. Delete cache and reconfigure!")
    endif()

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

    # Disable RTTI and exceptions.
    if(MSVC)
        remove_flags(CMAKE_CXX_FLAGS "/EHsc")
        append_flags(CMAKE_CXX_FLAGS "/EHs-c- /GR-")
        add_compile_definitions("-D_HAS_EXCEPTIONS=0")
    else()
        append_flags(CMAKE_CXX_FLAGS "-fno-rtti")
        append_flags(CMAKE_CXX_FLAGS "-fno-exceptions")
    endif()

    # Enable fast math by default.
    if(MSVC)
        append_flags(CMAKE_CXX_FLAGS "/fp:fast")
    else()
        append_flags(CMAKE_CXX_FLAGS "-ffast-math")
    endif()

    # Define custom set of configurations.
    set_cache(CMAKE_CONFIGURATION_TYPES "Debug;Develop;Release")

    # Add global defines for identifying each configuration from code.
    add_compile_definitions(
        $<$<CONFIG:Debug>:CMAKE_CONFIG_DEBUG>
        $<$<CONFIG:Develop>:CMAKE_CONFIG_DEVELOP>
        $<$<CONFIG:Release>:CMAKE_CONFIG_RELEASE>
    )

    # Make Release same as removed RelWithDebInfo configuration.
    set_cache(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_RC_FLAGS_RELEASE "${CMAKE_RC_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}")
    set_cache(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")

    # Make Develop behave like Release configuration (without additonal changes below).
    set_cache(CMAKE_ASM_FLAGS_DEVELOP "${CMAKE_ASM_FLAGS_RELEASE}")
    set_cache(CMAKE_C_FLAGS_DEVELOP "${CMAKE_C_FLAGS_RELEASE}")
    set_cache(CMAKE_CXX_FLAGS_DEVELOP "${CMAKE_CXX_FLAGS_RELEASE}")
    set_cache(CMAKE_RC_FLAGS_DEVELOP "${CMAKE_RC_FLAGS_RELEASE}")
    set_cache(CMAKE_STATIC_LINKER_FLAGS_DEVELOP "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}")
    set_cache(CMAKE_SHARED_LINKER_FLAGS_DEVELOP "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
    set_cache(CMAKE_EXE_LINKER_FLAGS_DEVELOP "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

    # Select MSVC runtime library based on configuration.
    if(MSVC)
        set_cache(CMAKE_MSVC_RUNTIME_LIBRARY "")
        append_flags(CMAKE_C_FLAGS_DEBUG "/MDd")
        append_flags(CMAKE_CXX_FLAGS_DEBUG "/MDd")
        append_flags(CMAKE_C_FLAGS_DEVELOP "/MD")
        append_flags(CMAKE_CXX_FLAGS_DEVELOP "/MD")
        append_flags(CMAKE_C_FLAGS_RELEASE "/MD")
        append_flags(CMAKE_CXX_FLAGS_RELEASE "/MD")
    endif()

    # Enable debugging info for all configurations.
    # For non-Release configurations enable hot reload.
    if(MSVC)
        set_cache(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "")
        append_flags(CMAKE_C_FLAGS_DEBUG "/ZI")
        append_flags(CMAKE_CXX_FLAGS_DEBUG "/ZI")
        append_flags(CMAKE_C_FLAGS_DEVELOP "/ZI")
        append_flags(CMAKE_CXX_FLAGS_DEVELOP "/ZI")
        append_flags(CMAKE_C_FLAGS_RELEASE "/Zi")
        append_flags(CMAKE_CXX_FLAGS_RELEASE "/Zi")
    endif()

    # More aggressive inline expansion for Release configuration.
    if(MSVC)
        remove_flags(CMAKE_C_FLAGS_RELEASE "/Ob1")
        remove_flags(CMAKE_CXX_FLAGS_RELEASE "/Ob1")
        append_flags(CMAKE_C_FLAGS_RELEASE "/Ob2")
        append_flags(CMAKE_CXX_FLAGS_RELEASE "/Ob2")
    endif()

    # Disable incremental linking for Release configuration.
    if(MSVC)
        remove_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        remove_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        append_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
        append_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
    endif()

    # Enable whole program optimization in Release configuration.
    if(MSVC)
        append_flags(CMAKE_C_FLAGS_RELEASE "/GL")
        append_flags(CMAKE_CXX_FLAGS_RELEASE "/GL")
        append_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
        append_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/LTCG")
        append_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG")
    endif()

    # Enable link optimizations in Release configuration.
    if(MSVC)
        append_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/OPT:REF")
        append_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:REF")
        append_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/OPT:ICF")
        append_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:ICF")
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
# Executable
#

function(setup_cmake_executable target) 
    if(WIN32)
        set_target_properties(${target} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")

        if(${target} STREQUAL "Tests")
            set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE FALSE)
        else()
            set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
        endif()
    endif()
endfunction()
