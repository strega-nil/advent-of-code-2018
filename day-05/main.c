#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/set.h>
#include <aoc/string.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool should_blast(char current, char next) {
  if (islower(current)) {
    return isupper(next) && tolower(next) == current;
  } else if (isupper(current)) {
    return islower(next) && toupper(next) == current;
  } else {
    return false;
  }
}

// returns whether the input was changed
bool blast_once(string *input) {
  char *write_iterator = input->start;
  char const *read_iterator = input->start;

  char const *const end = input->end;

  while (read_iterator != end) {
    char const *const read_iterator_next = read_iterator + 1;

    if (read_iterator_next == end) {
      *write_iterator++ = *read_iterator++;
    } else if (should_blast(*read_iterator, *read_iterator_next)) {
      // blast them!
      read_iterator += 2;
    } else {
      // don't blast it
      *write_iterator++ = *read_iterator++;
    }
  }

  if (input->end != write_iterator) {
    input->end = write_iterator;
    return true;
  } else {
    return false;
  }
}

void blast_all_elements(string *input, char ch) {
  if (!islower(ch)) {
    fprintf(stderr, "Invalid polymer unit: %c\n", ch);
    abort();
  }

  char *write_iterator = input->start;
  char const *read_iterator = input->start;

  char const *const end = input->end;

  while (read_iterator != end) {
    if (tolower(*read_iterator) == ch) {
      // blast it!
      ++read_iterator;
    } else {
      // don't blast it
      *write_iterator++ = *read_iterator++;
    }
  }

  input->end = write_iterator;
}

size_t length_of_reacted_polymer(string data) {
  for (;;) {
    if (!blast_once(&data)) {
      break;
    }
  }

  return string_length(data);
}

void do_the_thing(string data, enum part p) {
  // get rid of the trailing newline
  data = string_split_at(data, '\n').first;

  switch (p) {
  case PART_A: {
    printf("Reacted polymer has length: %zu\n",
           length_of_reacted_polymer(data));
  } break;
  case PART_B: {
    size_t minimum = SIZE_MAX;

    for (char ch = 'a'; ch <= 'z'; ++ch) {
      string tmp = string_copy(data);
      blast_all_elements(&tmp, ch);
      printf("Blasted %c: initial length is %zu\n", ch, string_length(tmp));
      size_t final = length_of_reacted_polymer(tmp);
      printf("Reacted polymer after %c blasted has length: %zu\n", ch, final);
      if (final < minimum) {
        minimum = final;
      }
    }

    printf("Shortest polymer has length: %zu\n", minimum);
  } break;
  }
}

DEFINE_MAIN(5)
