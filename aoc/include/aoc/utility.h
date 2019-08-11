#ifndef AOC_UTILITY_H
#define AOC_UTILITY_H

#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef int compare_t(void const *, void const *);

typedef void for_each_t(void *element, void *thunk);

enum part {
  PART_A,
  PART_B,
};

#define DEFINE_MAIN(number)                                                    \
  int main(int argc, char **argv) {                                            \
    if (argc < 2) {                                                            \
      fprintf(stderr, "choose part a or b\n");                                 \
      return 1;                                                                \
    }                                                                          \
    enum part part;                                                            \
    if (strcmp(argv[1], "a") == 0) {                                           \
      part = PART_A;                                                           \
    } else if (strcmp(argv[1], "b") == 0) {                                    \
      part = PART_B;                                                           \
    } else {                                                                   \
      fprintf(stderr, "invalid part: %s\n", argv[1]);                          \
      return 1;                                                                \
    }                                                                          \
    string data = aoc_get_input((number));                                     \
    if (not data.start) {                                                      \
      return 1;                                                                \
    }                                                                          \
    do_the_thing(data, part);                                                  \
    return 0;                                                                  \
  }

#define array_size(self) (sizeof(self) / sizeof(self[0]))

#define range_for_each(type, name, start, end)                                 \
  for (type name = (start), __##name##_end = (end); name < __##name##_end;     \
       ++name)

#define slice_for_each(type, name, start, end)                                 \
  for (type *name = (start), *__##name##_end = (end); name != __##name##_end;  \
       ++name)

#define array_for_each(type, name, self)                                       \
  slice_for_each(type, name, self, &(self)[array_size(self)])

#endif // AOC_UTILITY_H
