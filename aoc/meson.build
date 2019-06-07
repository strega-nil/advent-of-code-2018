aoc_includes = include_directories('include')

aoc = declare_dependency(
  link_with: library(
    'aoc',
    'src/read.c',
    'src/string.c',
    include_directories: aoc_includes,
  ),
  include_directories: aoc_includes,
)
