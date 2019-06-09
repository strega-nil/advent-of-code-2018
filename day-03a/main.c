#include <aoc/dynamic_buffer.h>
#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct claim {
  int id;
  int left;
  int top;
  int width;
  int height;
};

// returns {id = -1, ...} on error
struct claim parse_claim(string const line) {
  enum {
    BUFFER_LEN = 64,
  };

  struct claim ret = {-1};
  char buffer[BUFFER_LEN] = {0};

  if (string_length(line) > BUFFER_LEN - 1) {
    fprintf(
      stderr,
      "ill formed string; too long (%zu)",
      string_length(line)
    );
    return ret;
  }

  memcpy(buffer, line.start, string_length(line));
  // already zero-terminated

  int places_filled = sscanf(
    buffer,
    "#%d @ %d,%d: %dx%d",
    &ret.id, &ret.left, &ret.top, &ret.width, &ret.height
  );

  if (places_filled != 5) {
    fprintf(stderr, "ill formed string: %s", buffer);
    ret.id = -1;
    return ret;
  }

  return ret;
}

void do_the_thing(string const data) {
  struct claim* claims = db_new(struct claim);

  {
    struct lines_iterator it = lines_iterator_new(data);
    while (it.rest.start != it.rest.end) {
      lines_iterator_next(&it);

      struct claim claim = parse_claim(it.current);
      if (claim.id == -1) {
        continue;
      }
      db_push(claims, claim);
    }
  }

  enum {
    FABRIC_SIZE = 1000,
  };

  typedef short row[FABRIC_SIZE];
  row* fabric = calloc(FABRIC_SIZE, sizeof(*fabric));

  struct claim const* const claims_end = db_end(claims);
  for (struct claim const* it = claims; it != claims_end; ++it) {
    for (int x = it->left; x < it->left + it->width; ++x) {
      for (int y = it->top; y < it->top + it->height; ++y) {
        ++fabric[y][x];
      }
    }
  }

  db_free(claims);

  int count = 0;
  for (size_t x = 0; x < FABRIC_SIZE; ++x) {
    for (size_t y = 0; y < FABRIC_SIZE; ++y) {
      if (fabric[y][x] > 1) {
        ++count;
      }
    }
  }

  printf("Answer is %d\n", count);

  free(fabric);
}

int main() {
  string data = aoc_read(3, S("input.txt"));

  do_the_thing(data);

  return 0;
}
