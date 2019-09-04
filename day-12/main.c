#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/set.h>
#include <aoc/string.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct state {
	// note: always ordered
	int* live_indexes;

	/*
		bottom five bits of neighborhood are the neighborhood of index
		if the (index + n)-th plant is live, where -2 <= n <= 2, then
		that bit is 1; else, it's 0. All higher bits are always zero.
		i.e., neighborhood & 1 is the liveness of the (index + 2)-th plant

		0x01 -- index + 2
		0x02 -- index + 1
		0x04 -- index + 0
		0x08 -- index - 1
		0x10 -- index - 2
	*/
	unsigned char* maps_to_live;
};

int* parse_live_indexes(string line) {
	line = string_split_at(line, ':').rest;
	if (line.start == line.end || line.start + 1 == line.end) {
		fprintf(stderr, "invalid string\n");
		abort();
	}

	// skip the space
	line.start = line.start + 1;

	int* live_indexes = db_new(int);
	int index = 0;
	for (char* it = line.start; it != line.end; ++it, ++index) {
		if (*it == '#') {
			db_push(live_indexes, index);
		} else if (*it != '.') {
			if (*it == '\n') {
				break;
			} else {
				fprintf(stderr, "invalid input character: %c\n", *it);
				abort();
			}
		}
	}

	return live_indexes;
}

#define MAPS_TO_DEAD ((unsigned char)255)

unsigned char parse_map_to_live(string const line) {
	char input[5] = {0};
	char output = 0;

	int scanf_res = str_scanf(line, "%c%c%c%c%c => %c", &input[0], &input[1], &input[2],
	&input[3], &input[4], &output);

	if (scanf_res < 6) {
		goto ill_formed_line;
	}

	if (output == '.') {
		return MAPS_TO_DEAD;
	} else if (output == '#') {
		unsigned char result = 0;

		int index = 0;
		array_for_each(char, it, input) {
			if (*it == '#') {
				result |= 1 << (4 - index);
			} else if (*it != '.') {
				goto ill_formed_line;
			}
			++index;
		}

		return result;
	}

ill_formed_line:
	fprintf(stderr, "ill-formed line: %.*s\n", (int)string_length(line), line.start);
	abort();
}

struct state parse_state(string const data) {
	struct state result = {
		.live_indexes = NULL,
		.maps_to_live = db_new(unsigned char),
	};

	bool first = true;
	string_lines_for_each(line, data) {
		if (first) {
			result.live_indexes = parse_live_indexes(line);
			first = false;
		} else if (line.start == line.end) {
			continue;
		} else {
			unsigned char m = parse_map_to_live(line);
			if (m != MAPS_TO_DEAD) {
				db_push(result.maps_to_live, m);
			}
		}
	}

	return result;
}

bool should_live(int index, struct state const* state) {
	unsigned char neighborhood = 0;

	db_for_each(int, it, state->live_indexes) {
		int difference = index - *it;
		if (-2 <= difference && difference <= 2) {
			neighborhood |= 1 << (difference + 2);
		}
	}

	db_for_each(unsigned char, it, state->maps_to_live) {
		if (*it == neighborhood) {
			return true;
		}
	}

	return false;
}

void do_step(struct state* state) {
	if (db_is_empty(state->live_indexes)) {
		return;
	}

	const int leftmost = *state->live_indexes - 1;
	const int rightmost = db_end(state->live_indexes)[-1] + 1;

	int* result = db_new(int);
	range_for_each(int, index, leftmost, rightmost + 1) {
		if (should_live(index, state)) {
			db_push(result, index);
		}
	}

	db_free(state->live_indexes);
	state->live_indexes = result;
}

int64_t get_sum(int const* live_indexes) {
	int64_t result = 0;
	db_for_each(int const, index, live_indexes) {
		result += *index;
	}

	return result;
}

void do_part_a(struct state state) {
	range_for_each(int, time_step, 0, 20) {
		do_step(&state);
	}

	printf("\nsum is: %" PRId64 "\n", get_sum(state.live_indexes));
}

void do_part_b(struct state state) {
	// 50'000'000'000
	const int64_t states_total = 50000000000;
	int64_t differences[5] = {0};
	int64_t last_sum = 0;

	int64_t states_left = states_total;
	array_for_each(int64_t, diff, differences) {
		do_step(&state);
		int64_t sum = get_sum(state.live_indexes);
		*diff = sum - last_sum;
		last_sum = sum;
		--states_left;
	}

	for (;;) {
		do_step(&state);
		--states_left;

		int64_t current_sum = get_sum(state.live_indexes);
		int64_t current_diff = current_sum - last_sum;
		last_sum = current_sum;

		differences[0] = differences[1];
		differences[1] = differences[2];
		differences[2] = differences[3];
		differences[3] = differences[4];
		differences[4] = current_diff;

		bool all_same = true;
		array_for_each(int64_t, diff, differences) {
			if (current_diff != *diff) {
				all_same = false;
				break;
			}
		}

		if (all_same) {
			break;
		}
	}

	int64_t sum = get_sum(state.live_indexes);
	printf("Differences stabilized at step: %" PRIu64 "\n", states_total - states_left);
	printf("The sum at that point is: %" PRIi64 "\n", sum);
	int64_t difference = differences[0];
	printf("The difference at that point is: %" PRIi64 "\n", difference);

	sum += difference * states_left;
	printf("Sum is: %" PRIi64 "\n", sum);
}

void do_the_thing(string const data, enum part part) {
	struct state state = parse_state(data);

	(part == PART_A ? do_part_a : do_part_b)(state);
}

DEFINE_MAIN(12)
