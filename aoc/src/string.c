#include <aoc/string.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string string_copy(string s) {
  size_t length = s.end - s.start;
  string ret;

  if (!s.start) {
    ret.start = ret.end = NULL;
  } else {
    ret.start = malloc(length);
    ret.end = ret.start + length;

    memcpy(ret.start, s.start, length);
  }

  return ret;
}

char* string_to_cstring(string s) {
  size_t length = s.end - s.start;
  char* ret = malloc(length + 1);

  if (s.start) {
    memcpy(ret, s.start, length);
  }
  ret[length] = '\0';
  return ret;
}

string string_of_cstring(char const* s) {
  size_t length = strlen(s);

  string ret = {0};
  ret.start = malloc(length);

  if (not ret.start) {
    return ret;
  }

  ret.end = ret.start + length;

  {
    char* dst;
    char const* src;
    for (dst = ret.start, src = s; *src != '\0'; ++dst, ++src) {
      *dst = *src;
    }
  }

  return ret;
}

void string_free(string s) {
  free(s.start);
}

struct string_split string_split_at(string s, char ch) {
  string first = {0};
  first.end = first.start = s.start;

  while (first.end != s.end and *first.end != ch) {
    ++first.end;
  }

  if (first.end == s.end) {
    s.start = first.end;
  } else {
    s.start = first.end + 1;
  }

  struct string_split ret = {0};

  ret.first = first;
  ret.rest = s;

  return ret;
}

bool _Aoc_lines_iterator_next(string* current, string* rest) {
  if (rest->start == rest->end) {
    return false;
  }

  struct string_split split = string_split_at(*rest, '\n');
  *current = split.first;
  *rest = split.rest;

  return true;
}

int str_scanf(string s, char const* format, ...) {
  enum {
    SMALL_BUFFER_OPT = 64
  };
  va_list args;
  char buffer[SMALL_BUFFER_OPT] = {0};
  char* ptr;
  size_t const length = string_length(s);
  bool const use_small_buffer = length < SMALL_BUFFER_OPT - 1;

  if (use_small_buffer) {
    ptr = buffer;
  } else {
    ptr = calloc(length + 1, 1);
  }

  memcpy(ptr, s.start, length);
  // already zero-terminated

  va_start(args, format);
  int ret = vsscanf(ptr, format, args);
  va_end(args);

  if (not use_small_buffer) {
    free(ptr);
  }

  return ret;
}
