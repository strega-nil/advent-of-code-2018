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

  struct string_split line = {0};
  line.rest = data;

  for (;;) {
    line = string_split_at(line.rest, '\n');
    size_t const length = string_length(line.first);

    if (line.rest.start == line.rest.end) {
      // start over from the beginning
      line.rest = data;
    }

    if (length == 0) {
      continue;
    }

    if (length > 31) {
      fprintf(stderr, "line is too long (%zu)\n", length);
      goto end;
    }

    memcpy(buffer, line.first.start, length);
    buffer[length] = '\0';

    {
      int tmp;
      if (sscanf(buffer, "%d", &tmp) < 1) {
        fprintf(stderr, "line doesn't follow format: %s\n", buffer);
        goto end;
      }
      acc += tmp;
    }

    if (set_insert(frequencies, acc)) {
      printf("Answer is %d\n", acc);
      goto end;
    }
  }

end:
  set_free(frequencies);
}

int main() {
  string data = aoc_read(1, S("input.txt"));
  do_the_thing(data);

  return 0;
}
