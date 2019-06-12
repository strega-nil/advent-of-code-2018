#include <aoc/dynamic_buffer.h>
#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool close(string s1, string s2) {
  size_t const length = string_length(s1);
  if (length != string_length(s2)) {
    return false;
  }

  bool found_difference = false;
  for (size_t idx = 0; idx < length; ++idx) {
    if (s1.start[idx] != s2.start[idx]) {
      if (found_difference) {
        return false;
      }
      found_difference = true;
    }
  }

  return true;
}

// precondition - string_length(s1) == string_length(s2)
void find_solution(string s1, string s2) {
  size_t const length = string_length(s1);
  char* buffer = calloc(length + 1, 1);

  char* buffer_ptr = buffer;
  for (size_t idx = 0; idx < length; ++idx) {
    if (s1.start[idx] == s2.start[idx]) {
      *buffer_ptr = s1.start[idx];
      ++buffer_ptr;
    }
  }

  printf("solution: %s", buffer);
  free(buffer);
}

void do_the_thing(string data) {
  struct string_split line = {0};
  line.rest = data;

  string* line_buffer = db_new(string);

  for (;;) {
    line = string_split_at(line.rest, '\n');

    if (line.rest.start == line.rest.end) {
      break;
    }
    if (line.first.start == line.first.end) {
      continue;
    }

    for (
      string const* it = line_buffer;
      it != db_end(line_buffer);
      ++it
    ) {
      if (close(line.first, *it)) {
        printf("found the strings!\n");
        printf("s1: %.*s\n", (int)string_length(*it), it->start);
        printf(
          "s2: %.*s\n",
          (int)string_length(line.first),
          line.first.start
        );

        find_solution(line.first, *it);
        return;
      }
    }

    db_push(line_buffer, line.first);
  }

  fprintf(stderr, "not found!\n");
  for (string const* it = line_buffer; it != db_end(line_buffer); ++it) {
    fprintf(stderr, "%.*s", (int)(it->end - it->start), it->start);
  }
}

int main() {
  string data = aoc_read(2, S("input.txt"));
  do_the_thing(data);

  return 0;
}
