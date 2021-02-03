# Aery Engine
**Aery** is a work-in-progress game engine written in C++20. Its primary graphics backend is Vulkan, but OpenGL support is planned for the future, and DirectX12 is
also considered. Right now, the Vulkan renderer can render very simple shaders to the screen.

The engine will focus on 2D applications first, with full 3D support maybe coming at a later date.

## Building
Building on Windows should be very easy using Visual Studio 2019. You need to have the Vulkan SDK installed to your machine, and included in the project. The project
includes prebuilt Windows binaries for GLFW.

Most of the code is platform-independent, however I will **not** support platforms besides Windows as I do not have the required knowledge to do so.

Make sure to build with the C++20 compiler flag.

### Dependencies
* GLFW
* Vulkan-Hpp
* VulkanMemoryAllocator
* fmt
