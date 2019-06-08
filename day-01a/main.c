#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <string.h>

void do_the_thing(string const data) {
  int acc = 0;
  char buffer[32] = {0};

  struct get_line_return line = {0};
  line.rest = data;

  for (;;) {
    line = get_line(line.rest);
    size_t const length = string_length(line.line);

    if (length > 31) {
      goto invalid_format;
    }

    memcpy(buffer, line.line.start, length);
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
    (int)(line.line.end - line.line.start),
    line.line.start
  );
}

int main() {
  string data = aoc_read(1, S("input.txt"));
  do_the_thing(data);

  return 0;
}
