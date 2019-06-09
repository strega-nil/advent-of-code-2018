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

struct lines_iterator {
  string current;
  string rest;
};

struct lines_iterator lines_iterator_new(string s);
void lines_iterator_next(struct lines_iterator* it);

#define S(...) string_of_cstring(__VA_ARGS__)
#define string_length(s) ((size_t)((s).end - (s).start))

#endif // AOC_STRING_H
