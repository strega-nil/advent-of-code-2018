#ifndef AOC_STRING_H
#define AOC_STRING_H

#include <aoc/utility.h>

typedef struct string {
  char* start;
  char* end;
} string;

size_t string_length(string s);

char* string_to_cstring(string s);
string string_of_cstring(char const* s);

struct get_line_return {
  string line;
  string rest;
};

struct get_line_return get_line(string s);

#define S(...) string_of_cstring(__VA_ARGS__)

#endif // AOC_STRING_H
