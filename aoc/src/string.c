#include <aoc/string.h>

#include <stdlib.h>
#include <string.h>

size_t string_length(string s) {
  return s.end - s.start;
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

