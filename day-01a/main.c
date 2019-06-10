#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <string.h>

void do_the_thing(string const data) {
  int acc = 0;
  char buffer[32] = {0};

  struct string_split line = {0};
  line.rest = data;

  for (;;) {
    line = string_split_at(line.rest, '\n');
    size_t const length = string_length(line.first);

    if (length > 31) {
      goto invalid_format;
    }

    memcpy(buffer, line.first.start, length);
    buffer[length] = '\0';

    int tmp;
    if (sscanf(buffer, "%d", &tmp) < 1) {
      goto invalid_format;
    }
    acc += tmp;

    if (line.rest.start == line.rest.end) {
      break;
    }
  }

  printf("Answer is %d\n", acc);
  return;

invalid_format:
  fprintf(
    stderr,
    "Invalid format: %.*s",
    (int)(line.first.end - line.first.start),
    line.first.start
  );
}

int main() {
  string data = aoc_read(1, S("input.txt"));
  do_the_thing(data);

  return 0;
}
