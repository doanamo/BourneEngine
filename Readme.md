Bourne Engine
=============
[![Compile and Test](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml/badge.svg?branch=main)](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml)

Main repository for the Bourne game engine. This is a solo project written from scratch in a simple subset of C++. Architecture and design is very opinionated to serve more personal purposes first. For example, third-party libraries are to be avoided when possible to facilitate a learning process of implementing various systems and algorithms from ground up. This allows the codebase to be kept more consistent and binaries very small, while also leaving enough control over the entire implementation and architecture to help make it more optimized.

# Design
- Everything written in simple C++ from scratch
- Avoid third-party libraries and prefer native platforms APIs
- Focus on hardware utilization, memory efficiency and responsiveness
- Favor static dispatch over dynamic polymorphism
- Maintain unit tests to ensure correctness

# Requirements
Project is based on CMake 3.29 and can be used with following compilers:
- Clang (recommended 19.1 or higher)
- MSVC (recommended 19.42 or higher)
- GCC (recommended 14.2 or higher)

# Features
- **Build**
  - Three configurations:
    - Debug (no optimizations, for debugging)
    - Develop (regular optimizations, for development)
    - Release (maximum optimizations, for distribution)
  - Better defaults for compilation and link flags
  - Multi-platform and multi-compiler support
- **Common**
  - Logging
  - Assertions
  - Containers:
    - Array (aka "vector")
    - String, StringView
  - Utility:
    - Result (for more modern error handling)
    - Optional
    - Function
    - UniquePtr
- **Memory**
  - Allocator interface:
    - Default allocator based on malloc
    - Inline allocator for small allocations
  - Allocation statistics and tracking
  - Operator new/delete overrides
- **Platform**
  - Multi-platform support:
    - Linux
    - Windows
  - Command line handling
  - High frequency timing
  - Window management

# FAQ
### Is this just a toy project focused exclusively on self learning?
No. I am setting the bar quite high for myself by e.g. disallowing use of third party libraries to challenge myself and make sure this game engine ends up extremely well optimised. However, I am aiming to eventually make this engine a viable option for developing different kinds of games.
