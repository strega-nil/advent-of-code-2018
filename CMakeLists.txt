cmake_minimum_required(VERSION 3.12)

project(aoc-2018
	DESCRIPTION "Nicole's AoC 2018 solutions"
	LANGUAGES C)

if (MSVC)
	string(REGEX REPLACE "/W[0-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

function(set_options target)
	if (MSVC)
		target_compile_options(${target} PRIVATE -W4 -WX -wd4204 -wd4200)
		target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
	else()
		target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	endif()

	target_compile_features(${target} PRIVATE c_std_11)
endfunction()

file(GLOB_RECURSE aoc_sources CONFIGURE_DEPENDS aoc/src/*.c day-*/*.c)
add_executable(aoc ${aoc_sources})
target_include_directories(aoc PUBLIC aoc/include)
set_options(aoc)

file(GLOB days
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_SOURCE_DIR}
	day-*)

list(LENGTH days num_days)

set(define_num_days "static size_t const num_days = ${num_days}\\\\;")
set(define_days_type "typedef void (*day_t)(string, enum part)\\\\;")
set(define_days_array "static day_t days[${num_days}]\\\\;")

foreach(day RANGE 1 ${num_days})
	set(define_days "${define_days} extern day_t const day_${day}\\\\;")
	set(define_initialize_days "${define_initialize_days}days[${day}] = day_${day}\\\\;")
endforeach()

target_compile_definitions(aoc PRIVATE
	DECLARE_DAYS=${define_num_days}${define_days_type}${define_days}${define_days_array}
	INITIALIZE_DAYS=${define_initialize_days})

find_program(CLANG_FORMAT clang-format)
if (CLANG_FORMAT)
	file(GLOB_RECURSE format_sources CONFIGURE_DEPENDS
		aoc/*.c
		aoc/*.h
		day-*/*.c)
	add_custom_target(format COMMAND ${CLANG_FORMAT} -i -verbose ${format_sources})
endif()
