cmake_minimum_required(VERSION 3.7.0)

project(Lunar)

add_subdirectory(ext/glfw)
include_directories(ext/inc)
include_directories(ext/glfw/include)

find_package(Vulkan REQUIRED FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(VK_USE_DEBUG_LAYERS=1)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB FMT_IMPL "ext/inc/fmt/*.cc")

include_directories(inc)
add_executable(Lunar ${SOURCES} ${FMT_IMPL})

target_include_directories(Lunar
    PUBLIC ${VULKAN_INCLUDE_DIRS}
    PUBLIC ${GLFW_INCLUDE_DIR}
)

target_link_libraries(Lunar glfw ${GLFW_LIBRARIES})
target_link_libraries(Lunar Vulkan::Vulkan)
