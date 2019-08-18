#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/string.h>
#include <aoc/utility.h>

struct light {
	int x_position;
	int y_position;
	int x_velocity;
	int y_velocity;
};

struct light* parse_lights(string data) {
	struct light* result = db_new(struct light);

	string_lines_for_each(line, data) {
		if (line.start == line.end) {
			continue;
		}

		struct light tmp;
		if (str_scanf(
						line,
						"position=< %d,  %d> velocity=< %d,  %d>",
						&tmp.x_position,
						&tmp.y_position,
						&tmp.x_velocity,
						&tmp.y_velocity) != 4) {
			fprintf(stderr, "Invalid line\n");
			abort();
		}
		db_push(result, tmp);
	}

	return result;
}

void time_step(struct light* lights) {
	db_for_each(struct light, light, lights) {
		light->x_position += light->x_velocity;
		light->y_position += light->y_velocity;
	}
}

bool light_within_range_of_another(
		struct light initial, struct light* lights) {
	db_for_each(struct light, light, lights) {
		int x_distance = light->x_position - initial.x_position;
		int y_distance = light->y_position - initial.y_position;

		if (x_distance == -1 || x_distance == 1) {
			if (y_distance >= -1 && y_distance <= 1) {
				return true;
			}
		} else if (x_distance == 0) {
			if (y_distance == -1 || y_distance == 1) {
				return true;
			}
		}
	}

	return false;
}

bool any_at_position(struct light* lights, int x, int y) {
	db_for_each(struct light, light, lights) {
		if (light->x_position == x && light->y_position == y) {
			return true;
		}
	}

	return false;
}

void do_the_thing(string data) {
	struct light* lights = parse_lights(data);

	for (int i = 0;; ++i) {
		bool all_within_range = true;
		db_for_each(struct light, light, lights) {
			if (!light_within_range_of_another(*light, lights)) {
				all_within_range = false;
				break;
			}
		}

		if (all_within_range) {
			printf("At time %d, the message is:\n", i);
			break;
		}

		time_step(lights);
	}

	int least_x = INT_MAX;
	int least_y = INT_MAX;
	int greatest_x = INT_MIN;
	int greatest_y = INT_MIN;

	db_for_each(struct light, light, lights) {
		if (light->x_position < least_x) {
			least_x = light->x_position;
		}
		if (light->y_position < least_y) {
			least_y = light->y_position;
		}

		if (light->x_position > greatest_x) {
			greatest_x = light->x_position;
		}
		if (light->y_position > greatest_y) {
			greatest_y = light->y_position;
		}
	}

	for (int y = least_y; y <= greatest_y; ++y) {
		for (int x = least_x; x <= greatest_x; ++x) {
			if (any_at_position(lights, x, y)) {
				putchar('#');
			} else {
				putchar('.');
			}
		}

		putchar('\n');
	}
}

DEFINE_MAIN_NOPART(10)
