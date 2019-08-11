#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/set.h>
#include <aoc/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int int_compare(void const *lhs_, void const *rhs_) {
  int lhs = *(int const *)lhs_;
  int rhs = *(int const *)rhs_;

  if (lhs < rhs) {
    return -1;
  } else if (lhs == rhs) {
    return 0;
  } else {
    return 1;
  }
}

void do_the_thing(string const data, enum part part) {
  char buffer[32] = {0};

  int *frequency_changes = db_new(int);

  int frequency = 0;
  set frequencies = set_new(int_compare);
  set_insert(frequencies, frequency);

  lines_for_each(line, data) {
    size_t const length = string_length(line);

    if (length == 0) {
      continue;
    }

    if (length > 31) {
      fprintf(stderr, "line is too long (%zu)\n", length);
      goto end;
    }

    memcpy(buffer, line.start, length);
    buffer[length] = '\0';

    {
      int tmp;
      if (sscanf(buffer, "%d", &tmp) < 1) {
        fprintf(stderr, "line doesn't follow format: %s\n", buffer);
        goto end;
      }
      db_push(frequency_changes, tmp);
      frequency += tmp;
    }

    set_insert(frequencies, frequency);
  }

  switch (part) {
  case PART_A:
    printf("Answer is %d\n", frequency);
    goto end;
  case PART_B: {
    int const *const frequency_changes_end = db_end(frequency_changes);

    for (int const *it = frequency_changes;; ++it) {
      if (it == frequency_changes_end) {
        it = frequency_changes;
      }

      frequency += *it;
      if (set_insert(frequencies, frequency)) {
        printf("Answer is %d\n", frequency);
        goto end;
      }
    }
  }
  }

end:
  set_free(frequencies);
  db_free(frequency_changes);
}

DEFINE_MAIN(1)
