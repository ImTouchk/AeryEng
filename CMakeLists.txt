cmake_minimum_required(VERSION 3.7.0)

project(Lunar)

add_subdirectory(ext/glfw)
include_directories(ext/glfw/include)

find_package(Vulkan REQUIRED FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(inc)
add_library(Lunar STATIC ${SOURCES})

target_include_directories(Lunar
    PUBLIC ${VULKAN_INCLUDE_DIRS}
    PUBLIC ${GLFW_INCLUDE_DIR}
)

target_link_libraries(Lunar glfw ${GLFW_LIBRARIES})
target_link_libraries(Lunar Vulkan::Vulkan)
