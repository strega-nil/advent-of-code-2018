#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/string.h>
#include <aoc/utility.h>

struct node {
	int* metadata;
	struct node* children;
};

// NOTE: it's not necessarily required, but in order to catch bugs, I
// put a -1 at the end of the int list.

int* parse_int_list(string data) {
	int* result = db_new(int);

	string_lines_for_each(line, data) {
		// skip empty lines
		if (line.start == line.end) {
			continue;
		}

		int tmp;
		if (str_scanf(line, "%d", &tmp) != 1) {
			fprintf(stderr, "Malformed line\n");
			abort();
		}
		db_push(result, tmp);
	}

	db_push(result, -1);

	return result;
}

#define CHECK(x)                                                       \
	do {                                                                 \
		if (!(x)) {                                                        \
			fprintf(stderr, "%d: CHECK FAILURE: %s", __LINE__, #x);          \
			abort();                                                         \
		}                                                                  \
	} while (0)

struct parse_node_result {
	struct node node;
	int const* new_it;
};

struct parse_node_result parse_node(int const* it) {
	int child_node_count = *it++;
	CHECK(child_node_count != -1);
	int metadata_count = *it++;
	CHECK(metadata_count != -1);

	struct parse_node_result result = {
			.node = {.metadata = db_new(int),
							 .children = db_new(struct node)},
			.new_it = NULL};

	for (int i = 0; i < child_node_count; ++i) {
		struct parse_node_result child = parse_node(it);
		it = child.new_it;
		db_push(result.node.children, child.node);
	}

	for (int i = 0; i < metadata_count; ++i) {
		int meta = *it++;
		CHECK(meta != -1);
		db_push(result.node.metadata, meta);
	}

	result.new_it = it;
	return result;
}

int sum_metadata(struct node const* node) {
	int sum = 0;

	db_for_each(int, meta, node->metadata) { sum += *meta; }
	db_for_each(struct node, child, node->children) {
		sum += sum_metadata(child);
	}

	return sum;
}

int node_value(struct node const* node) {
	if (db_is_empty(node->children)) {
		int sum = 0;
		db_for_each(int, meta, node->metadata) { sum += *meta; }
		return sum;
	} else {
		int sum = 0;
		db_for_each(int, meta, node->metadata) {
			if (*meta == 0) {
				continue;
			} else if (*meta > db_length(node->children)) {
				continue;
			}

			sum += node_value(&node->children[*meta - 1]);
		}
		return sum;
	}
}

void do_the_thing(string data, enum part p) {
	(void)p;

	int* numbers = parse_int_list(data);

	struct parse_node_result node_result = parse_node(numbers);
	CHECK(node_result.new_it == db_end(numbers) - 1);

	struct node tree = node_result.node;

	int (*f)(struct node const*) =
			p == PART_A ? sum_metadata : node_value;
	printf("Result is: %d", f(&tree));
}

DEFINE_MAIN(8)
