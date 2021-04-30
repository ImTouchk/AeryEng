project(
  'Lunar', 
  'cpp', 
  version : '0.1', 
  default_options : ['warning_level=3']
)

########################
# Set C++ standard
########################

if meson.get_compiler('cpp').get_id() == 'msvc'
  message('Building with MSVC...')
  standard = 'cpp_std=c++latest'
else
  standard = 'cpp_std=c++2a'
endif

########################
# Find dependencies
########################

glfw_dep = dependency('glfw3', required : false)
if not glfw_dep.found()
  cmake     = import('cmake')
  glfw_opt  = cmake.subproject_options()
  glfw_opt.add_cmake_defines({'BUILD_SHARED_LIBS':'OFF'})
  glfw_opt.set_override_option('cpp_std', 'c++latest')
  glfw_proj = cmake.subproject('glfw', options : glfw_opt)

  glfw_dep  = glfw_proj.dependency('glfw') 
endif

fmt_dep = dependency('fmt')

deps = [
  dependency('vulkan'), 
  dependency('threads'),
  glfw_dep,
  fmt_dep
]

########################
# Create library file
########################

header_files = include_directories(
  'include'
)

source_files = files([
  'source/graphics/renderer.cpp',
  'source/graphics/vk_boilerplate.cpp',
  'source/graphics/window.cpp',
])

library = static_library(
  'Lunar', 
  source_files,
  include_directories : header_files,
  dependencies : deps,
  install : false,
  override_options : [standard]
)

########################
# Make example program
########################

test_app = executable(
  'TestApp',
  'examples/main.cpp',
  include_directories : header_files,
  link_with : library,
  dependencies : deps,
  install : false,
  override_options : [standard]
)
