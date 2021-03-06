# Lunar Engine
**Lunar** is a work-in-progress game engine written in C++20 using the new module system. The engine targets mainly Windows (x86 and x64), but the code should be cross-platform. The graphics backend used is Vulkan, although I might consider adding DirectX12 support at a later date.

Keep in mind that as this project is using the new module system, so development will not be as stable as usual.

## Building
Building on Windows should be straight forward as long as you use the latest version of Visual Studio. You need to have the Vulkan SDK installed to your machine, and included in the project. The `inc` and `lib` folders bundle most of the dependencies (`lib` contains precompiled libraries for VS)

### Dependencies
* GLFW
* Vulkan SDK
* VulkanMemoryAllocator
* fmt
* glm
