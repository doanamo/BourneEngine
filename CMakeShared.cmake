cmake_minimum_required(VERSION 3.28)

set(CURRENT_CACHE_VERSION 1)

function(print_variable variable)
    message(STATUS "${variable} = ${${variable}}")
endfunction()

function(set_cache variable value)
    string(STRIP "${value}" stripped)
    set(${variable} ${stripped} CACHE INTERNAL "")
endfunction()

function(add_flags variable flags)
    string(FIND "${${variable}}" "${flags}" found)
    if(found EQUAL -1)
        set_cache(${variable} "${${variable}} ${flags}")
    endif()
endfunction()

function(remove_flags variable flags)
    string(FIND "${${variable}}" "${flags}" found)
    if(NOT found EQUAL -1)
        string(REPLACE "${flags}" "" removed "${${variable}}")
        string(REPLACE "  " " " removed "${removed}")
        set_cache(${variable} "${removed}")
    endif()
endfunction()

function(shared_cmake_setup)
    # Prevent this function from running more than once.
    if(CUSTOM_CMAKE_SETUP_CALLED)
        message(FATAL_ERROR "Custom CMake setup already called!")
    endif()

    set(CUSTOM_CMAKE_SETUP_CALLED TRUE PARENT_SCOPE)

    # Version cache in case of breaking changes that require cache reset.
    if(NOT CACHE_VERSION)
        set_cache(CACHE_VERSION ${CURRENT_CACHE_VERSION})
    elseif(NOT CACHE_VERSION EQUAL ${CURRENT_CACHE_VERSION})
        message(FATAL_ERROR "Cache version mismatch. Reset cache and reconfigure!")
    endif()

    # Enable unity build.
    set(CMAKE_UNITY_BUILD ON)

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
        add_flags(CMAKE_CXX_FLAGS "/EHs-c- /GR-")
        add_compile_definitions("-D_HAS_EXCEPTIONS=0")
    else()
        add_flags(CMAKE_CXX_FLAGS "-fno-rtti")
        add_flags(CMAKE_CXX_FLAGS "-fno-exceptions")
    endif()

    # Enable fast math by default.
    if(MSVC)
        add_flags(CMAKE_CXX_FLAGS "/fp:fast")
    else()
        add_flags(CMAKE_CXX_FLAGS "-ffast-math")
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
    set_cache(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO}")
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
    set_cache(CMAKE_MODULE_LINKER_FLAGS_DEVELOP "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}")
    set_cache(CMAKE_EXE_LINKER_FLAGS_DEVELOP "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

    # Parity for flags between different linker types.
    if(MSVC)
        add_flags(CMAKE_STATIC_LINKER_FLAGS_DEBUG "/debug")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_DEVELOP "/debug")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/debug")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_DEBUG "/INCREMENTAL")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_DEVELOP "/INCREMENTAL")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/INCREMENTAL")
    endif()

    # Enable debugging info for all configurations.
    # For non-Release configurations enable hot reload.
    if(MSVC)
        add_flags(CMAKE_C_FLAGS_DEBUG "/ZI")
        add_flags(CMAKE_CXX_FLAGS_DEBUG "/ZI")
        add_flags(CMAKE_C_FLAGS_DEVELOP "/ZI")
        add_flags(CMAKE_CXX_FLAGS_DEVELOP "/ZI")
        add_flags(CMAKE_C_FLAGS_RELEASE "/Zi")
        add_flags(CMAKE_CXX_FLAGS_RELEASE "/Zi")
     endif()

    # Enable whole program optimization in Release configuration.
    if(MSVC)
        add_flags(CMAKE_C_FLAGS_RELEASE "/GL")
        add_flags(CMAKE_CXX_FLAGS_RELEASE "/GL")
        remove_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        remove_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        remove_flags(CMAKE_MODULE_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        remove_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
        add_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
        add_flags(CMAKE_MODULE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
        add_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
        add_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/LTCG")
        add_flags(CMAKE_MODULE_LINKER_FLAGS_RELEASE "/LTCG")
        add_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG")
    else()
        add_flags(CMAKE_C_FLAGS_RELEASE "-flto")
        add_flags(CMAKE_CXX_FLAGS_RELEASE "-flto")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "-flto")
        add_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-flto")
        add_flags(CMAKE_MODULE_LINKER_FLAGS_RELEASE "-flto")
        add_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")
    endif()

    # Enable link optimizations in Release configuration.
    if(MSVC)
        add_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/OPT:REF")
        add_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/OPT:REF")
        add_flags(CMAKE_MODULE_LINKER_FLAGS_RELEASE "/OPT:REF")
        add_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:REF")
        add_flags(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/OPT:ICF")
        add_flags(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/OPT:ICF")
        add_flags(CMAKE_MODULE_LINKER_FLAGS_RELEASE "/OPT:ICF")
        add_flags(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:ICF")
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
        print_variable(CMAKE_MODULE_LINKER_FLAGS)
        print_variable(CMAKE_EXE_LINKER_FLAGS)

        print_variable(CMAKE_ASM_FLAGS_DEBUG)
        print_variable(CMAKE_C_FLAGS_DEBUG)
        print_variable(CMAKE_CXX_FLAGS_DEBUG)
        print_variable(CMAKE_RC_FLAGS_DEBUG)
        print_variable(CMAKE_STATIC_LINKER_FLAGS_DEBUG)
        print_variable(CMAKE_SHARED_LINKER_FLAGS_DEBUG)
        print_variable(CMAKE_MODULE_LINKER_FLAGS_DEBUG)
        print_variable(CMAKE_EXE_LINKER_FLAGS_DEBUG)

        print_variable(CMAKE_ASM_FLAGS_DEVELOP)
        print_variable(CMAKE_C_FLAGS_DEVELOP)
        print_variable(CMAKE_CXX_FLAGS_DEVELOP)
        print_variable(CMAKE_RC_FLAGS_DEVELOP)
        print_variable(CMAKE_STATIC_LINKER_FLAGS_DEVELOP)
        print_variable(CMAKE_SHARED_LINKER_FLAGS_DEVELOP)
        print_variable(CMAKE_MODULE_LINKER_FLAGS_DEVELOP)
        print_variable(CMAKE_EXE_LINKER_FLAGS_DEVELOP)

        print_variable(CMAKE_ASM_FLAGS_RELEASE)
        print_variable(CMAKE_C_FLAGS_RELEASE)
        print_variable(CMAKE_CXX_FLAGS_RELEASE)
        print_variable(CMAKE_RC_FLAGS_RELEASE)
        print_variable(CMAKE_STATIC_LINKER_FLAGS_RELEASE)
        print_variable(CMAKE_SHARED_LINKER_FLAGS_RELEASE)
        print_variable(CMAKE_MODULE_LINKER_FLAGS_RELEASE)
        print_variable(CMAKE_EXE_LINKER_FLAGS_RELEASE)
    endif()
endfunction()
