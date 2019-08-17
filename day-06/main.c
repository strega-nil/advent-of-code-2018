#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/set.h>
#include <aoc/string.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pair {
	int x;
	int y;
};

struct pair parse_pair(string line) {
	char buffer[64];

	size_t const length = string_length(line);
	if (length > 63) {
		fprintf(stderr, "line is too long: %zu\n", length);
		abort();
	}

	memcpy(buffer, line.start, length);
	buffer[length] = '\0';

	struct pair res;
	int scanf_res = sscanf(buffer, "%d, %d", &res.x, &res.y);

	if (scanf_res < 2) {
		fprintf(stderr, "invalid line: %s", buffer);
		abort();
	}

	return res;
}

typedef struct pair* pair_ptr;

// starts at (0, 0)
struct map {
	int right;
	int top;
	struct pair** buffer;
};

pair_ptr* index_map(struct map map, int x, int y) {
	int const width = map.right;
	return &map.buffer[width * y + x];
}

int calculate_distance(struct pair pair, int x, int y) {
	int const x_dist = abs(x - pair.x);
	int const y_dist = abs(y - pair.y);

	return x_dist + y_dist;
}

struct map make_map(struct pair* list) {
	if (db_is_empty(list)) {
		fprintf(stderr, "make_map() doesn't work on empty lists\n");
		abort();
	}

	struct map result = {0, 0, NULL};

	db_for_each(struct pair, pair, list) {
		if (pair->x >= result.right) {
			result.right = pair->x + 1;
		}
		if (pair->y >= result.top) {
			result.top = pair->y + 1;
		}
	}

	size_t total_size = (size_t)(result.right) * (size_t)(result.top);

	result.buffer = calloc(total_size, sizeof(*result.buffer));

	range_for_each(int, y, 0, result.top) {
		range_for_each(int, x, 0, result.right) {
			struct pair* closest = NULL;
			int distance = INT_MAX;
			db_for_each(struct pair, pair, list) {
				const int pair_dist = calculate_distance(*pair, x, y);
				if (pair_dist < distance) {
					closest = pair;
					distance = pair_dist;
				} else if (pair_dist == distance) {
					closest = NULL;
					distance = INT_MIN;
				}
			}

			*index_map(result, x, y) = closest;
		}
	}

	return result;
}

// returns -1 on infinite area
int calculate_area_of(struct map* map, struct pair* current) {
	// assume a board which is shaped like:
	//    +y
	//     |
	// -x --- +x
	//     |
	//    -y

	// if a pair owns an edge, we have an infinite area
	range_for_each(int, y, 0, map->top) {
		if (*index_map(*map, 0, y) == current) {
			return -1;
		}
		if (*index_map(*map, map->right - 1, y) == current) {
			return -1;
		}
	}
	range_for_each(int, x, 0, map->right) {
		if (*index_map(*map, x, 0) == current) {
			return -1;
		}
		if (*index_map(*map, x, map->top - 1) == current) {
			return -1;
		}
	}

	int area = 0;
	range_for_each(int, y, 1, map->top - 1) {
		range_for_each(int, x, 1, map->right - 1) {
			if (*index_map(*map, x, y) == current) {
				++area;
			}
		}
	}

	return area;
}

struct pair* parse_list(string data) {
	struct pair* result = db_new(struct pair);

	string_lines_for_each(line, data) {
		if (line.start == line.end)
			continue;
		struct pair tmp = parse_pair(line);
		db_push(result, tmp);
	}

	return result;
}

void do_part_a(struct pair* pairs) {
	struct map map = make_map(pairs);

	int greatest_area = -1;
	db_for_each(struct pair, pair, pairs) {
		int area = calculate_area_of(&map, pair);
		if (area == -1) {
			printf("Pair (%d, %d) has infinite area\n", pair->x, pair->y);
		} else if (area > greatest_area) {
			printf(
					"Pair (%d, %d) is the largest area so far: %d\n",
					pair->x,
					pair->y,
					area);
			greatest_area = area;
		} else {
			printf(
					"Pair (%d, %d) has finite area: %d\n",
					pair->x,
					pair->y,
					area);
		}
	}

	printf("Greatest area is: %d\n", greatest_area);
}

#define SAFE_DISTANCE 10000
bool within_safe_distance(struct pair* pairs, int x, int y) {
	int distance = 0;

	db_for_each(struct pair, pair, pairs) {
		distance += calculate_distance(*pair, x, y);
		if (distance >= SAFE_DISTANCE) {
			return false;
		}
	}

	return true;
}

void do_part_b(struct pair* pairs) {
	int max_x = 0;
	int max_y = 0;

	db_for_each(struct pair, pair, pairs) {
		if (pair->x + 1 > max_x) {
			max_x = pair->x + 1;
		}
		if (pair->y + 1 > max_y) {
			max_y = pair->y + 1;
		}
	}

	int area = 0;
	range_for_each(int, y, 0, max_y) {
		range_for_each(int, x, 0, max_x) {
			if (within_safe_distance(pairs, x, y)) {
				++area;
			}
		}
	}

	printf("Region of safety has area %d\n", area);
}

void do_the_thing(string data, enum part p) {
	struct pair* pairs = parse_list(data);

	void (*f)(struct pair*) = p ? do_part_a : do_part_b;
	f(pairs);
}

DEFINE_MAIN(6)
