#include <aoc/read.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char*
aoc_read_filename(int day, string filename) {
  char* ret = NULL;

  char const* const format = "data-%02d/";
  size_t const folder_length = snprintf(NULL, 0, format, day);
  size_t const filename_length = string_length(filename);

  if (filename_length == 0) {
    return NULL;
  }

  size_t const ret_length = folder_length + filename_length;

  ret = malloc(ret_length + 1);
  if (not ret) {
    return NULL;
  }

  snprintf(ret, folder_length + 1, format, day);
  memcpy(ret + folder_length, filename.start, filename_length);
  ret[ret_length] = '\0';

  return ret;
}

string aoc_read(int day, string filename) {
  char* full_filename = aoc_read_filename(day, filename);
  FILE* file = fopen(full_filename, "r");
  if (not file) {
    perror(full_filename);
    return (string){0};
  }

  free(full_filename);


  size_t length = 32;
  char* start = malloc(length);
  char* end = start;

  char buffer[32];

  for (;;) {
    size_t size = fread(buffer, 1, 32, file);
    if (size < 32) {
      // end of file
      memcpy(end, buffer, size);
      end += size;
      break;
    } else {
      assert(size == 32);
      memcpy(end, buffer, 32);
      end += 32;
      if (end == start + length) {
        char* tmp = malloc(length * 2);
        memcpy(tmp, start, length);
        start = tmp;
        end = start + length;
        length *= 2;
      }
    }
  }

  fclose(file);
  return (string){start, end};
}
