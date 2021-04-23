project(
  'Lunar', 
  'cpp', 
  version : '0.1', 
  default_options : ['warning_level=3', 'cpp_std=c++2a']
)

deps = [
  dependency('vulkan'), 
  dependency('glfw3'),
  dependency('threads')
]

header_files = include_directories(
  'include'
)

source_files = [
  'source/window.cpp'
]

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
  dependencies: deps,
  override_options : ['cpp_std=c++2a']
)
