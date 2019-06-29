#include <aoc/dynamic_buffer.h>
#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct value_of_return {
  bool has_two;
  bool has_three;
};

struct value_of_return value_of(string s) {
  int buffer[26] = {0};

  struct value_of_return ret = {0};

  string_for_each (it, s) {
    int index = *it - 'a';
    if (0 <= index and index < 26) {
      ++buffer[index];
    } else {
      fprintf(
        stderr,
        "invalid string: %.*s",
        (int)string_length(s),
        s.start
      );
      return ret;
    }
  }

  array_for_each (int const, it, buffer) {
    if (*it == 3) {
      ret.has_three = true;
    }
    if (*it == 2) {
      ret.has_two = true;
    }
  }

  return ret;
}

bool are_close(string s1, string s2) {
  size_t const length = string_length(s1);
  if (length != string_length(s2)) {
    return false;
  }

  bool found_difference = false;
  for (size_t idx = 0; idx < length; ++idx) {
    if (s1.start[idx] != s2.start[idx]) {
      if (found_difference) {
        return false;
      }
      found_difference = true;
    }
  }

  return true;
}

void find_solution_for_a(string* buffer) {
  int num_twos = 0;
  int num_threes = 0;

  db_for_each(string const, s, buffer) {
    struct value_of_return v = value_of(*s);
    num_twos += v.has_two;
    num_threes += v.has_three;
  }
  printf("Answer is %d\n", num_twos * num_threes);
}

void print_difference_of_strings(string s1, string s2) {
  size_t const length = string_length(s1);

  if (length != string_length(s2)) {
    fprintf(
      stderr,
      "String lengths are not the same: %zu, %zu\n",
      length,
      string_length(s2)
    );
  }

  printf("1st string is %.*s\n", (int)string_length(s1), s1.start);
  printf("2nd string is %.*s\n", (int)string_length(s2), s2.start);

  char* buffer = calloc(length + 1, 1);

  char* buffer_ptr = buffer;
  for (size_t idx = 0; idx < length; ++idx) {
    if (s1.start[idx] == s2.start[idx]) {
      *buffer_ptr = s1.start[idx];
      ++buffer_ptr;
    }
  }

  printf("Answer is %s\n", buffer);
  free(buffer);
}

void find_solution_for_b(string* buffer) {
  string const* const buffer_end = db_end(buffer);
  slice_for_each(string const, a, buffer, buffer_end) {
    slice_for_each(string const, b, a + 1, buffer_end) {
      if (are_close(*a, *b)) {
        print_difference_of_strings(*a, *b);
        return;
      }
    }
  }

  fprintf(stderr, "not found!\n");
}

void do_the_thing(string data, enum part part) {
  string* line_buffer = db_new(string);

  lines_for_each(line, data) {
    if (line.start == line.end) {
      continue;
    }
    db_push(line_buffer, line);
  }

  switch (part) {
    case PART_A:
      find_solution_for_a(line_buffer);
      break;
    case PART_B:
      find_solution_for_b(line_buffer);
      break;
  }
}

DEFINE_MAIN(2)
