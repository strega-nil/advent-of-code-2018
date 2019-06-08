#include <aoc/dynamic_buffer.h>
#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct foo {int x;};

int main() {
  //string data = aoc_read(2, S("input.txt"));

  void* x = &(struct foo){0};
  (void)x;

  return 0;
}
