#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void do_the_thing(string const data) {
  int acc = 0;
  char buffer[32] = {0};

  size_t capacity = 4096;
  int* freq_buffer = calloc(capacity, sizeof(*freq_buffer));
  int* freq_buffer_end = freq_buffer + 1;

  string rest_of_lines = data;

  for (;;) {
    string const line = get_line(rest_of_lines);
    size_t const length = string_length(line);

    if (length == 0) {
      if (line.end == data.end) {
        // start over from the beginning
        rest_of_lines.start = data.start;
      }
      continue;
    }
    if (length > 31) {
      goto invalid_format;
    }

    memcpy(buffer, line.start, length);
    buffer[length] = '\0';

    {
      int tmp;
      if (sscanf(buffer, "%d", &tmp) < 1) {
        goto invalid_format;
      }
      acc += tmp;
    }

    rest_of_lines.start = line.end + 1;

    for (int const* it = freq_buffer; it != freq_buffer_end; ++it) {
      if (acc == *it) {
        printf("Answer is %d\n", acc);
        return;
      }
    }

    if (freq_buffer_end == freq_buffer + capacity) {
      int* tmp = calloc(capacity * 2, sizeof(*freq_buffer));
      memcpy(tmp, freq_buffer, capacity * sizeof(*freq_buffer));
      freq_buffer = tmp;
      freq_buffer_end = freq_buffer + capacity;
      capacity *= 2;
    }

    *freq_buffer_end++ = acc;

    continue;
  invalid_format:
    fprintf(stderr, "Invalid format: %.*s", (int)length, line.start);
    return;
  }
}

int main() {
  string data = aoc_read(1, S("input.txt"));
  do_the_thing(data);

  return 0;
}
