project('aoc',
  'c',
  license: 'Apache-2.0',
  default_options: [
    'werror=true',
    'warning_level=3',
    'c_std=c11',
    'default_library=static',
  ],
)

compiler = meson.get_compiler('c').get_id()
if compiler == 'msvc'
  add_project_arguments(
    '-wd4204',
    '-wd4200',
    '-D_CRT_SECURE_NO_WARNINGS',
    language: 'c',
  )
endif

subdir('aoc')

subdir('day-1a')
subdir('day-1b')
