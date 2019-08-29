#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/set.h>
#include <aoc/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct position {
	int x;
	int y;
};

int power_of(struct position pos, int grid_serial) {
	int const rack_id = pos.x + 10;

	int power_level = rack_id * pos.y;
	power_level += grid_serial;
	power_level *= rack_id;

	// only keep the 100s digit
	int real_power_level = (power_level / 100) % 10;
	return real_power_level - 5;
}

struct square_max_sum {
	struct position top_left;
	int sum;
};

struct square_max_sum max_of_size(int size, int (*grid)[300][300]) {
	struct square_max_sum result = {
		.top_left.x = -1,
		.top_left.y = -1,
		.sum = INT_MIN
	};

	/*
		there's definitely a better way to do this where one only
		recalculates for the borders
		_make certain_ to build in release mode if you don't want to grab
		coffee while you wait
	*/
	for (int top = 0; top < 300 - size; ++top) {
		for (int left = 0; left < 300 - size; ++left) {
			int sum = 0;
			for (int diff_y = 0; diff_y < size; ++diff_y) {
				for (int diff_x = 0; diff_x < size; ++diff_x) {
					sum += (*grid)[top + diff_y][left + diff_x];
				}
			}

			if (sum > result.sum) {
				result.sum = sum;
				result.top_left.x = left + 1;
				result.top_left.y = top + 1;
			}
		}
	}

	return result;
}

void do_the_thing(string const data, enum part part) {
	int grid_serial;
	if (str_scanf(data, "%d", &grid_serial) != 1) {
		printf("Incorrectly formatted data: %s\n", string_to_cstring(data));
	}

	int(*grid)[300][300] = malloc(sizeof(*grid));

	for (int y = 0; y < 300; ++y) {
		for (int x = 0; x < 300; ++x) {
			struct position pos = {.x = x + 1, .y = y + 1};
			(*grid)[y][x] = power_of(pos, grid_serial);
		}
	}

	switch (part) {
		case PART_A: {
			struct square_max_sum sum = max_of_size(3, grid);

			printf(
					"The max sum is at position (%d, %d), and is %d",
					sum.top_left.x,
					sum.top_left.y,
					sum.sum);
		} break;
		case PART_B: {
			struct square_max_sum max_sum = {
				.top_left.x = -1,
				.top_left.y = -1,
				.sum = INT_MIN
			};
			int max_size = -1;

			for (int size = 1; size <= 300; ++size) {
				printf("checking size: %d\n", size);
				struct square_max_sum sum = max_of_size(size, grid);

				if (sum.sum > max_sum.sum) {
					max_sum = sum;
					max_size = size;
				}
			}
			printf(
					"The max sum is at position (%d, %d) with size %d, and is %d",
					max_sum.top_left.x,
					max_sum.top_left.y,
					max_size,
					max_sum.sum);
		}
	}

}

DEFINE_MAIN(11)
