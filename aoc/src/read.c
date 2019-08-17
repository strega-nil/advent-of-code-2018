#include <aoc/read.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* aoc_input_filename(int day) {
	char* ret = NULL;

	char const* const format = "day-%02d/input.txt";
	size_t const length = snprintf(NULL, 0, format, day);

	ret = malloc(length + 1);
	if (not ret) {
		return NULL;
	}

	snprintf(ret, length + 1, format, day);
	ret[length] = '\0';

	return ret;
}

string aoc_get_input(int day) {
	char* full_filename = aoc_input_filename(day);
	if (not full_filename) {
		return (string){0};
	}
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
