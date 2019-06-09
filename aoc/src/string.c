#include <aoc/string.h>

#include <stdlib.h>
#include <string.h>

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

struct get_line_return get_line(string s) {
  string line = {0};
  line.end = line.start = s.start;

  while (line.end != s.end and *line.end != '\n') {
    ++line.end;
  }

  if (line.end == s.end) {
    s.start = line.end;
  } else {
    s.start = line.end + 1;
  }

  struct get_line_return ret = {0};

  ret.line = line;
  ret.rest = s;

  return ret;
}

struct lines_iterator lines_iterator_new(string s) {
  struct lines_iterator ret = {0};
  ret.rest = s;

  return ret;
}

void lines_iterator_next(struct lines_iterator* it) {
  struct get_line_return gl = get_line(it->rest);
  it->current = gl.line;
  it->rest = gl.rest;
}
