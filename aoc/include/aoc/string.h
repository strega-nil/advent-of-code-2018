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

struct string_split {
  string first;
  string rest;
};

struct string_split string_split_at(string s, char ch);

struct lines_iterator {
  string current;
  string rest;
};

struct lines_iterator lines_iterator_new(string s);
void lines_iterator_next(struct lines_iterator* it);

#ifdef _MSC_VER
int str_scanf(
  string s,
  _Scanf_format_string_params_(0) char const* fmt,
  ...
);
#else
int str_scanf(string s, char const* fmt, ...)
  __attribute__((format(scanf, 2, 3)));
#endif

#define S(...) string_of_cstring(__VA_ARGS__)
#define string_length(s) ((size_t)((s).end - (s).start))

#endif // AOC_STRING_H
