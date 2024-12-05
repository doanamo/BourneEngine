Bourne Engine
=============
[![Compile and Test](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml/badge.svg?branch=main)](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml)

Main repository for the Bourne game engine. This is a solo project written from scratch in a simple subset of C++. Architecture and design is very opinionated to serve more personal purposes first. For example, third-party libraries are to be completely avoided to facilitate a learning process of implementing various systems and algorithms from ground up. This allows the codebase to be kept more consistent and binaries very small, while also leaving enough control over the entire implementation and architecture to help make it more optimized.

# Features and design
- Everything written in simple C++ from scratch
- Avoids third-party libraries and prefers cross-platform APIs
- Focus on hardware utilization, memory efficiency and responsiveness

# Requirements
- CLion (recommended 2024.3 or higher, with bundled CMake/Ninja)
- Visual Studio 2022 (recommended 17.11.5 or higher, with bundled CMake/Ninja)

# FAQ
### Is this just a toy project focused exclusively on self learning?
No. I am setting the bar quite high for myself by e.g. disallowing use of third party libraries to challenge myself and make sure this game engine ends up extremely well optimised. I am aiming to eventually make it a viable option for developing different kinds of games.

### Why was the engine named Bourne?
I really love the Jason Bourne series of movies. I usually never name my projects and instead assigned them some generic designation such as "Game Engine" because I have trouble coming up with good names that I feel like sticking to. I decided to assign a name this time around to break the habit and give the engine some character/identity for good luck.
