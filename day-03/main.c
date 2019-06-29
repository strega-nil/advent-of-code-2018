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

  struct claim ret = {0};
  ret.id = -1;

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

void do_the_thing(string const data, enum part part) {
  struct claim* claims = db_new(struct claim);

  lines_for_each(line, data) {
    struct claim claim = parse_claim(line);
    if (claim.id == -1) {
      continue;
    }
    db_push(claims, claim);
  }

  enum {
    FABRIC_SIZE = 1000,
  };

  typedef short row[FABRIC_SIZE];
  row* fabric = calloc(FABRIC_SIZE, sizeof(*fabric));

  db_for_each(struct claim const, it, claims) {
    range_for_each(int, y, it->top, it->top + it->height) {
      range_for_each(int, x, it->left, it->left + it->width) {
        ++fabric[y][x];
      }
    }
  }

  switch (part) {
    case PART_A: {
      int count = 0;
      range_for_each(int, y, 0, FABRIC_SIZE) {
        range_for_each(int, x, 0, FABRIC_SIZE) {
          if (fabric[y][x] > 1) {
            ++count;
          }
        }
      }
      printf("The answer is %d\n", count);
    } break;
    case PART_B: {
      db_for_each(struct claim const, it, claims) {
        bool overlapping = false;
        range_for_each(int, y, it->top, it->top + it->height) {
          range_for_each(int, x, it->left, it->left + it->width) {
            if (fabric[y][x] > 1) {
              overlapping = true;
            }
          }
        }
        if (not overlapping) {
          printf("Found the claim! ID #%d\n", it->id);
        }
      }
    } break;
  }


  free(fabric);
  db_free(claims);
}

DEFINE_MAIN(3)
