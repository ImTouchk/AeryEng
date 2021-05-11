cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(Lunar VERSION 0.0.1 DESCRIPTION "Lunar Engine")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# -------------- Dependencies
add_subdirectory(submodules/fmt)
add_subdirectory(submodules/tomlplusplus)
add_subdirectory(submodules/glfw)
add_subdirectory(submodules/glad)
find_package(Vulkan REQUIRED)

# -------------- Build library
add_library(Lunar SHARED
	inc/core/common.h
	inc/graphics/window.h
	src/graphics/window.cpp
	inc/graphics/renderer.h
	src/graphics/renderer.cpp
	inc/graphics/gl_renderer.h
	src/graphics/gl_renderer.cpp
)
target_include_directories(Lunar PUBLIC inc)
target_include_directories(Lunar PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Lunar PRIVATE tomlplusplus glfw Vulkan::Vulkan glad)
target_link_libraries(Lunar PUBLIC fmt)
target_compile_features(Lunar PUBLIC cxx_std_20)

if(DEFINED BUILD_EXAMPLE AND BUILD_EXAMPLE EQUAL "OFF")
	message("-- Not building example...")
else()
	message("-- Building example...")
	add_executable(lunar_test examples/main.cpp)
	target_link_libraries(lunar_test PUBLIC Lunar)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT lunar_test)
endif()
