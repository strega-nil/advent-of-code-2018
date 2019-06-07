#include <aoc/dynamic_buffer.h>
#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void do_the_thing(string const data) {
  int acc = 0;
  char buffer[32] = {0};

  int* freq_buffer = db_new_with_capacity(int, 8);
  db_push(freq_buffer, 0);

  string rest_of_lines = data;

  for (;;) {
    string const line = get_line(rest_of_lines);
    size_t const length = string_length(line);

    if (line.end == data.end) {
      // start over from the beginning
      rest_of_lines.start = data.start;
    } else {
      rest_of_lines.start = line.end + 1;
    }

    if (length == 0) {
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

    {
      int const* it;
      int const* end = db_end(freq_buffer);
      for (it = freq_buffer; it != end; ++it) {
        if (acc == *it) {
          printf("Answer is %d\n", acc);
          goto success;
        }
      }
    }

    db_push(freq_buffer, acc);

    continue;
  invalid_format:
    fprintf(stderr, "Invalid format: %.*s", (int)length, line.start);
    return;
  }

success:
  db_free(freq_buffer);
}

int main() {
  string data = aoc_read(1, S("input.txt"));
  do_the_thing(data);
  free(data.start);

  return 0;
}
