Bourne Engine
=============
Repository for the Bourne game engine. This is a personal project written from scratch in C++ that is very opiniated. Third-party libraries are to be completly avoided to facilitate a learning process of implementing various features/containers/algorithms/systems from ground up. This allows the codebase to be kept very small and simple, while also leaving enough control over the entire implementation/architecture to help make it extremely fast.

# Features and design
- Written from scratch in simple C++
- Extremely fast and very small
- Almost everything is custom
- Support for Windows only

# Requirements
- Visual Studio 2022 (recommended 17.11.5 or higher)
  - CMake (bundled with Visual Studio, recommended 3.29.5 or higher)
  - Ninja (bundled with Visual Studio, recommended 1.11.0 or higher)

# FAQ
### Is this just a toy project focused exlusively on self learning?
No. I am putting the bar quite high for myself by e.g. disallowing use of third party libraries to challenge myself and make sure Bourne game engine ends up extremely well optimized. I have professional programmng experience with game engines and I am aiming to make this a viable option for making different kinds of games in the far future.

### Why use Direct3D 11 instead of Direct3D 12 or Vulkan?
I believe new generation graphics APIs are way too overly complicated. I've seen far too many AAA games implement Direct3D 12 and Vulkan poorly, despite having dedicated teams of rendering engineers and backing from GPU vendors. I've also seen implementations of Vulkan in native Linux ports that run much worse than their Windows/Direct3D 11 versions when run under translation layers such as Proton.

### Why only support Windows?
I am not a big fan of Windows at all, but truth is that this is by far the most popular platform in gaming. Writting native cross platform support is a big undertaking that doesn't make sense from the business perspective where Linux has a tiny percentage of the player base. Not providing native builds for Linux is no longer a problem as solutions such as Proton are very good at running Windows games on Linux with pretty great performance.

### Why is the engine names Bourne?
I really love the Jason Bourne series of movies. I usually never name my projects. I always named them something generic such as "Game Engine" because I have trouble coming up with a good names that I feel like sticking to. I decided to name this project this time around to break the habit and give the engine some character/identity, for good luck.
