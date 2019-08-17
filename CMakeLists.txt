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

file(GLOB_RECURSE aoc_sources CONFIGURE_DEPENDS aoc/src/*.c)
add_library(aoc STATIC ${aoc_sources})
target_include_directories(aoc PUBLIC aoc/include)
set_options(aoc)

file(GLOB days
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_SOURCE_DIR}
	day-*)

foreach(day ${days})
	file(GLOB_RECURSE ${day}_sources ${day}/*.c)
	add_executable(${day} ${${day}_sources})
	target_link_libraries(${day} aoc)
	set_options(${day})
endforeach()

find_program(CLANG_FORMAT clang-format)
if (CLANG_FORMAT)
	file(GLOB_RECURSE format_sources CONFIGURE_DEPENDS
		aoc/*.c
		aoc/*.h
		day-*/*.c)
	add_custom_target(format COMMAND ${CLANG_FORMAT} -i -verbose ${format_sources})
endif()
