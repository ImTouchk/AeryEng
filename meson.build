project(
  'Lunar', 
  'cpp', 
  version : '0.1', 
  default_options : ['warning_level=3', 'cpp_std=c++2a']
)

deps = [
  dependency('vulkan'), 
  dependency('glfw3'),
  dependency('threads'),
  dependency('fmt')
]

header_files = include_directories(
  'include'
)

source_files = files([
  'source/graphics/renderer.cpp',
  'source/graphics/vk_boilerplate.cpp',
  'source/graphics/window.cpp',
])

library = shared_library(
  'Lunar', 
  source_files,
  include_directories : header_files,
  dependencies : deps,
  install : false
)

test_app = executable(
  'TestApp',
  'examples/main.cpp',
  include_directories : header_files,
  link_with : library,
  dependencies: deps
)
