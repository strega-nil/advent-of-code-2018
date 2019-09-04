#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/set.h>
#include <aoc/string.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum Direction {
	UP = 0x1,
	DOWN = 0x2,
	LEFT = 0x4,
	RIGHT = 0x8,

	// spaces
	NONE = 0x0,
	// |
	VERTICAL = UP | DOWN,
	// -
	HORIZONTAL = LEFT | RIGHT,
	// +
	JUNCTION = VERTICAL | HORIZONTAL,

	// these two are temporary, used only for curves
	// this is for /
	TEMPORARY_CURVE_FORWARD = 0x10,
	/* this is for \ */
	TEMPORARY_CURVE_BACKWARD = 0x20,
};

struct Track {
	enum Direction connections;
	struct Train* train;
};

enum Turn {
	TURN_LEFT,
	STRAIGHT,
	TURN_RIGHT,
};

enum Turn next_turn(enum Turn in) {
	if (in == TURN_RIGHT) {
		return TURN_LEFT;
	} else {
		return in + 1;
	}
}

enum Direction make_turn(enum Direction original, enum Turn by) {
	switch (by) {
		case STRAIGHT: return original;
		case TURN_LEFT: switch (original) {
			case UP: return LEFT;
			case DOWN: return RIGHT;
			case LEFT: return DOWN;
			case RIGHT: return UP;
			default: fprintf(stderr, "invalid direction: %d\n", original); abort();
		}
		case TURN_RIGHT: switch (original) {
			case UP: return RIGHT;
			case DOWN: return LEFT;
			case LEFT: return UP;
			case RIGHT: return DOWN;
			default: fprintf(stderr, "invalid direction: %d\n", original); abort();
		}
		default: fprintf(stderr, "invalid turn: %d\n", by); abort();
	}

}

struct Train {
	enum Direction direction;
	enum Turn next_turn;
	bool moved_already;
};

struct Map {
	struct Track* map;
	int width;
	int height;
};

struct Track* Map_get(struct Map const* map, int x, int y) {
	return &map->map[y * map->width + x];
}

struct Map parse_map(string const data) {
	struct Map result = {
		.map = db_new(struct Track),
		.width = 0,
		.height = 0,
	};

	// calculate width and height first
	string_lines_for_each(line, data) {
		if (line.start == line.end) {
			continue;
		}

		const int new_width = (int)(line.end - line.start);
		if (result.width < new_width) {
			result.width = new_width;
		}
		++result.height;
	}

	string_lines_for_each(line, data) {
		int width_left = result.width;
		slice_for_each(char, ch, line.start, line.end) {
			--width_left;
			struct Track track = {
				.connections = NONE,
				.train = NULL,
			};

			struct Train train = {
				.direction = NONE,
				.next_turn = TURN_LEFT,
				.moved_already = false,
			};

			switch (*ch) {
			case '|': track.connections = VERTICAL; break;
			case '-': track.connections = HORIZONTAL; break;
			case '+': track.connections = JUNCTION; break;
			case ' ': break;
			case '/': track.connections = TEMPORARY_CURVE_FORWARD; break;
			case '\\': track.connections = TEMPORARY_CURVE_BACKWARD; break;
			case '>': track.connections = HORIZONTAL; train.direction = RIGHT; break;
			case '<': track.connections = HORIZONTAL; train.direction = LEFT; break;
			case '^': track.connections = VERTICAL; train.direction = UP; break;
			case 'v': track.connections = VERTICAL; train.direction = DOWN; break;
			default: fprintf(stderr, "invalid char: %c\n", *ch); abort();
			}

			if (train.direction != NONE) {
				track.train = malloc(sizeof(track.train));
				*track.train = train;
			}

			db_push(result.map, track);
		}

		for (int i = 0; i < width_left; ++i) {
			struct Track track = { .connections = NONE, .train = NULL };
			db_push(result.map, track);
		}
	}

	for (int y = 0; y < result.height; ++y) {
		for (int x = 0; x < result.width; ++x) {
			enum Direction* dir = &Map_get(&result, x, y)->connections;

			// \ - backslash
			// either up-right or down-left
			if (*dir == TEMPORARY_CURVE_BACKWARD) {
				if (x == 0 || y == result.height - 1) {
					// can't be down-left
					*dir = UP | RIGHT;
				} else if (y == 0 || x == result.width - 1) {
					// can't be up-right
					*dir = DOWN | LEFT;
				} else {
					// we can check all around without worrying
					if (Map_get(&result, x, y - 1)->connections & DOWN) {
						// connection up
						*dir = UP | RIGHT;
					} else {
						*dir = DOWN | LEFT;
					}
				}
			}
			// / - forward slash
			// either down-right or up-left
			else if (*dir == TEMPORARY_CURVE_FORWARD) {
				if (x == 0 || y == 0) {
					// can't be up-left
					*dir = DOWN | RIGHT;
				} else if (y == result.height - 1 || x == result.width - 1) {
					// can't be down-right
					*dir = UP | LEFT;
				} else {
					// we can check all around without worrying
					if (Map_get(&result, x, y - 1)->connections & DOWN) {
						// connection up
						*dir = UP | LEFT;
					} else {
						*dir = DOWN | RIGHT;
					}
				}
			}
		}
	}

	return result;
}

struct Position {
	int x;
	int y;
};

// returns {-1, -1} if there's no collision
// else returns the position of the collision
struct Position do_tick(struct Map* map) {
	struct Position result = {-1, -1};

	for (int y = 0; y < map->height; ++y) {
		for (int x = 0; x < map->width; ++x) {
			struct Track* cur_track = Map_get(map, x, y);
			struct Train* cur_train = cur_track->train;
			if (!cur_train || cur_train->moved_already) {
				continue;
			}

			int new_x = x;
			int new_y = y;
			enum Direction from_which = NONE;
			switch (cur_train->direction) {
			case UP: new_y = y - 1; from_which = DOWN; break;
			case DOWN: new_y = y + 1; from_which = UP; break;
			case LEFT: new_x = x - 1; from_which = RIGHT; break;
			case RIGHT: new_x = x + 1; from_which = LEFT; break;
			default: fprintf(stderr, "wtf: %d\n", cur_train->direction); abort();
			}

			if (new_y < 0 || new_y >= map->height) {
				fprintf(stderr, "invalid train on y");
			} else if (new_x < 0 || new_x >= map->width) {
				fprintf(stderr, "invalid train on x");
			}

			struct Track* new_track = Map_get(map, new_x, new_y);
			if (new_track->train) {
				// found collision
				cur_track->train = NULL;
				new_track->train = NULL;
				if (result.x == -1) {
					result.x = new_x;
					result.y = new_y;
				}
			} else {
				cur_track->train = NULL;
				new_track->train = cur_train;
				cur_train->moved_already = true;

				if (new_track->connections == JUNCTION) {
					cur_train->direction = make_turn(cur_train->direction, cur_train->next_turn);
					cur_train->next_turn = next_turn(cur_train->next_turn);
				} else {
					cur_train->direction = new_track->connections & (~from_which);
				}
			}
		}
	}

	db_for_each(struct Track, track, map->map) {
		if (track->train) {
			track->train->moved_already = false;
		}
	}

	return result;
}

// returns the position {-1, number_of_trains} if there's more than one
// train; else returns the position of that last train
struct Position last_train(struct Map const* map) {
	struct Position result = {-1, -1};

	int number_of_trains = 0;

	for (int y = 0; y < map->height; ++y) {
		for (int x = 0; x < map->width; ++x) {
			if (Map_get(map, x, y)->train) {
				if (number_of_trains == 0) {
					result.x = x;
					result.y = y;
				}

				++number_of_trains;
			}
		}
	}

	if (number_of_trains != 1) {
		return (struct Position){-1, number_of_trains};
	} else {
		return result;
	}
}

void do_the_thing(string const data, enum part part) {
	struct Map map = parse_map(data);

	if (part == PART_A) {
		for (int tick = 0;; ++tick) {
			struct Position pos_of_crash = do_tick(&map);
			if (pos_of_crash.x == -1) {
				printf("tick %d: no crash\n", tick);
			} else {
				printf("tick %d: crash at (%d, %d)\n", tick, pos_of_crash.x, pos_of_crash.y);
				break;
			}
		}
	} else {
		for (int tick = 0;; ++tick) {
			struct Position crash = do_tick(&map);
			if (crash.x != -1) {
				printf("tick %d: crash at (%d, %d)\n", tick, crash.x, crash.y);
				struct Position pos = last_train(&map);
				if (pos.x != -1) {
					printf("  position of last train: (%d, %d)\n", pos.x, pos.y);
					break;
				} else {
					printf("  number of trains left: %d\n", pos.y);
				}
			}
		}
	}
}

DEFINE_MAIN(13)
