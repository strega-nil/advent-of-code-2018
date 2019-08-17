#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/string.h>
#include <aoc/utility.h>

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

struct marble {
	uint64_t number;
	struct marble* clockwise;
	struct marble* counter;
};

struct player {
	uint64_t score;
};

struct marble* insert_marble(
		struct marble* current, struct player* current_player, int number) {

	if (number % 23 != 0) {
		struct marble* new_current = malloc(sizeof(*new_current));

		new_current->number = number;

		new_current->counter = current->clockwise;
		new_current->clockwise = new_current->counter->clockwise;

		new_current->counter->clockwise = new_current;
		new_current->clockwise->counter = new_current;

		return new_current;
	} else {
		current_player->score += number;
		for (int i = 0; i < 7; ++i) {
			current = current->counter;
		}
		current_player->score += current->number;
		struct marble* new_current = current->clockwise;

		current->clockwise->counter = current->counter;
		current->counter->clockwise = current->clockwise;
		free(current);

		return new_current;
	}
}

void do_the_thing(string data, enum part p) {
	(void)p;

	struct marble* current = malloc(sizeof(*current));
	current->number = 0;
	current->clockwise = current;
	current->counter = current;

	int num_players;
	int last_marble;

	if (str_scanf(
					data,
					"%d players; last marble is worth %d points",
					&num_players,
					&last_marble) != 2) {
		fprintf(stderr, "malformed input");
		abort();
	}

	if (p == PART_B) {
		last_marble *= 100;
	}

	struct player* players =
			db_new_with_capacity(struct player, num_players);
	for (int i = 0; i < num_players; ++i) {
		db_push(players, (struct player){0});
	}

	int current_player = 1;
	for (int marble_num = 1; marble_num <= last_marble; ++marble_num) {
		current =
				insert_marble(current, &players[current_player], marble_num);
		current_player = (current_player + 1) % num_players;
	}

	uint64_t max = 0;
	db_for_each(struct player, player, players) {
		if (player->score > max) {
			max = player->score;
		}
	}

	printf("max score was: %" PRIu64 "\n", max);
}

DEFINE_MAIN(9)
