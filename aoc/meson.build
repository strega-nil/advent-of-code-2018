aoc_includes = include_directories('include')

aoc = declare_dependency(
  link_with: library(
    'aoc',
    'src/dynamic_buffer.c',
    'src/read.c',
    'src/set.c',
    'src/string.c',
    include_directories: aoc_includes,
  ),
  include_directories: aoc_includes,
)
