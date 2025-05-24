Bourne Engine
=============
[![Compile and Test](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml/badge.svg?branch=main)](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml)

Main repository for the Bourne game engine. This is a solo project written from scratch in a simple subset of C++. Architecture and design is very opinionated to serve more personal purposes first. For example, third-party libraries are to be avoided when possible to facilitate a learning process of implementing various game engine systems and algorithms from ground up. This will allow the codebase to be kept more consistent and binaries very small, while also leaving enough control over the entire implementation and architecture to help make it as lightweight and optimized as possible.

# Design
- Everything written in simple C++ from scratch
- Minimal use of third-party libraries with preference for native platforms APIs
- Focus on hardware utilization, memory efficiency and responsiveness
- Custom containers and standard types to support more robust memory handling
- Unit tests framework and suite to ensure correctness

# Requirements
Project is developed primarily for the Windows platform. Linux is currently supported only for running unit tests.

Builds via CMake 3.29 with following compilers:
- MSVC (recommended 19.42 or higher)
- Clang (recommended 19.1 or higher)
- GCC (recommended 14.2 or higher)

# Features
- **Build**
  - Three configurations:
    - Debug (no optimizations, for debugging)
    - Develop (regular optimizations, for development)
    - Release (maximum optimizations, for distribution)
  - Better defaults for compilation and link flags
  - Multi-platform and multi-compiler support
- **Memory**
  - Allocator interface:
    - Default allocator (selects the best allocator for size/type)
    - Inline allocator (for optimizing out heap allocations)
  - Allocation statistics and tracking
- **Common**
  - Logging
  - Assertions
  - Containers:
    - Array (aka resizable vector)
    - String (with small string optimization)
    - StringView
  - Utility:
    - Function (supports methods and lambda captures)
    - Result (for more modern error handling)
    - Optional
    - UniquePtr
- **Platform**
  - Command line handling
  - High precision timing
  - Window management
- **Graphics**
  - Direct3D 11 rendering
- **Testing**
  - Unit testing framework (with CTest integration)

# Contact
For any inquires regarding this project, please contact me via: bourne.stonewall319@passfwd.com
