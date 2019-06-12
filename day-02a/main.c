#include <aoc/dynamic_buffer.h>
#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct value_of_return {
  bool has_two;
  bool has_three;
};

struct value_of_return value_of(string s) {
  int buffer[26] = {0};

  struct value_of_return ret = {0};

  for (char const* it = s.start; it != s.end; ++it) {
    int index = *it - 'a';
    if (0 <= index and index < 26) {
      ++buffer[index];
    } else {
      fprintf(
        stderr,
        "invalid string: %.*s",
        (int)string_length(s),
        s.start
      );
      return ret;
    }
  }

  for (int const* it = &buffer[0]; it != &buffer[26]; ++it) {
    if (*it == 3) {
      ret.has_three = true;
    }
    if (*it == 2) {
      ret.has_two = true;
    }
  }

  return ret;
}

void do_the_thing(string data) {
  struct string_split line = {0};
  line.rest = data;

  int num_twos = 0;
  int num_threes = 0;

  for (;;) {
    line = string_split_at(line.rest, '\n');

    struct value_of_return v = value_of(line.first);

    num_twos += v.has_two;
    num_threes += v.has_three;

    if (line.rest.start == line.rest.end) {
      printf("Answer is %d\n", num_twos * num_threes);
      break;
    }
  }
}

int main() {
  string data = aoc_read(2, S("input.txt"));
  do_the_thing(data);

  return 0;
}
