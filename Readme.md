Bourne Engine
=============
[![Compile and Test](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml/badge.svg?branch=main)](https://github.com/doanamo/BourneEngine/actions/workflows/CompileAndTest.yml)

Main repository for the Bourne game engine. This is a solo project written from scratch in a simple sebset of C++. Architecture and design is very opiniated to serve more personal purposes first. For example, third-party libraries are to be completely avoided to facilitate a learning process of implementing various systems and algorithms from ground up. This allows the codebase to be kept more consistent and binaries very small, while also leaving enough control over the entire implementation and architecture to help make it more optimized.

# Features and design
- Written from scratch in simple C++
- Almost everything is custom instead of relying on third party libraries
- Specialized rather than generalized
- Support for Windows only

# Requirements
- Visual Studio 2022 (recommended 17.11.5 or higher)
  - CMake (bundled with Visual Studio, recommended 3.29.5 or higher)
  - Ninja (bundled with Visual Studio, recommended 1.11.0 or higher)

# FAQ
### Is this just a toy project focused exclusively on self learning?
No. I am setting the bar quite high for myself by e.g. disallowing use of third party libraries to challenge myself and make sure this game engine ends up extremely well optimised. I am aiming to eventually make it a viable option for developing different kinds of games.

### Why use Direct3D 11 instead of Direct3D 12 or Vulkan?
I believe new generation graphics APIs are way too overly complicated for a solo project. I've seen far too many AAA games implement Direct3D 12 and Vulkan poorly, despite having dedicated teams of rendering engineers and backing from GPU vendors. I've also seen implementations of Vulkan in native Linux ports that run much worse than their Windows/Direct3D 11 versions when run under translation layers such as Proton.

### Why only support Windows?
I am not a big fan of Windows at all, but the truth is that this is by far the most popular platform in gaming. Writing native cross platform support is a big undertaking that doesn't make sense from the business perspective, where Linux has a tiny percentage of the player base. Not providing native builds for Linux is no longer a problem as solutions such as Proton are very good at running Windows games on Linux with pretty great performance.

### Why was the engine named Bourne?
I really love the Jason Bourne series of movies. I usually never name my projects and instead assigned them some generic designation such as "Game Engine" because I have trouble coming up with good names that I feel like sticking to. I decided to assign a name this time around to break the habit and give the engine some character/identity for good luck.
