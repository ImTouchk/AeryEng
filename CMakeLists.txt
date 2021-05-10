cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(Lunar VERSION 0.0.1 DESCRIPTION "Lunar Engine")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# -------------- Dependencies
add_subdirectory(submodules/fmt)
add_subdirectory(submodules/toml11)
add_subdirectory(submodules/glfw)
find_package(Vulkan REQUIRED)

# -------------- Build library
add_library(Lunar SHARED 
	src/main.cpp
)
target_include_directories(Lunar PUBLIC inc)
target_include_directories(Lunar PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Lunar PRIVATE fmt toml11 glfw Vulkan::Vulkan)

if(DEFINED BUILD_EXAMPLE AND BUILD_EXAMPLE EQUAL "OFF")
	message("-- Not building example...")
else()
	message("-- Building example...")
	add_executable(lunar_test examples/main.cpp)
	target_link_libraries(lunar_test PUBLIC Lunar)
endif()
