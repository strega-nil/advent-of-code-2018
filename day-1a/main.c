#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <string.h>

string get_line(string s) {
  string ret = {0};
  ret.end = ret.start = s.start;

  while (ret.end != s.end and *ret.end != '\n') {
    ++ret.end;
  }

  return ret;
}

void do_the_thing(string data) {
  int acc = 0;
  char buffer[32] = {0};

  for (;;) {
    string const line = get_line(data);
    size_t const length = string_length(line);

    if (length > 31) {
      fprintf(stderr, "Invalid format: %.*s", (int)length, line.start);
      return;
    }

    memcpy(buffer, line.start, length);
    buffer[length] = '\0';

    int tmp;
    if (sscanf(buffer, "%d", &tmp) < 1) {
      fprintf(stderr, "Invalid format: %.*s", (int)length, line.start);
      return;
    }
    acc += tmp;

    if (line.end == data.end || line.end + 1 == data.end) {
      break;
    }
    data.start = line.end + 1;
  }

  printf("Answer is %d\n", acc);
}

int main() {
  string data = aoc_read(1, S("input.txt"));
  do_the_thing(data);

  return 0;
}
