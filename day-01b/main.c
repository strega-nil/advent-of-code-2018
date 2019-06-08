#include <aoc/read.h>
#include <aoc/set.h>
#include <aoc/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int int_compare(void const* lhs_, void const* rhs_) {
  int lhs = *(int const*)lhs_;
  int rhs = *(int const*)rhs_;

  if (lhs < rhs) {
    return -1;
  } else if (lhs == rhs) {
    return 0;
  } else {
    return 1;
  }
}

void do_the_thing(string const data) {
  int acc = 0;
  char buffer[32] = {0};

  set frequencies = set_new(int_compare);
  {int const tmp = 0; set_insert(frequencies, tmp);}
  int set_size = 1;

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

    if (set_insert(frequencies, acc)) {
      printf("Answer is %d\n", acc);
      goto success;
    }
    ++set_size;

    continue;
  invalid_format:
    fprintf(stderr, "Invalid format: %.*s", (int)length, line.start);
    return;
  }

success:
  set_free(frequencies);
}

int main() {
  string data = aoc_read(1, S("input.txt"));
  do_the_thing(data);
  free(data.start);

  return 0;
}