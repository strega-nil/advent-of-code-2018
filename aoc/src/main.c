#include <aoc/read.h>
#include <aoc/string.h>
#include <aoc/utility.h>

DECLARE_DAYS

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "choose a day\n");
		return 1;
	} else if (argc < 3) {
		fprintf(stderr, "choose part a or b\n");
		return 1;
	}

	INITIALIZE_DAYS

	int day;
	if (sscanf(argv[1], "%d", &day) != 1) {
		fprintf(stderr, "Invalid day: %s\n", argv[1]);
		return 1;
	}

	enum part part;
	if (strcmp(argv[2], "a") == 0) {
		part = PART_A;
	} else if (strcmp(argv[2], "b") == 0) {
		part = PART_B;
	} else {
		fprintf(stderr, "invalid part: %s\n", argv[2]);
		return 1;
	}

	string data = aoc_get_input(day);
	if (not data.start) {
		return 1;
	}

	if (day > num_days) {
		fprintf(stderr, "invalid day: %s\n", argv[2]);
	} else {
		days[day](data, part);
	}

	return 0;
}
