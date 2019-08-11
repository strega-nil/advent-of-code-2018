#include <aoc/dynamic_buffer.h>
#include <aoc/read.h>
#include <aoc/string.h>
#include <aoc/utility.h>

#include <ctype.h>
#include <stdlib.h>

struct step {
  char name;
  bool finished;
  char *preconditions;
};

struct step_relationship {
  // always between 'A' and 'Z'
  char precondition;
  char postcondition;
};

struct step_relationship *parse_relationships(string data) {
  struct step_relationship *result = db_new(struct step_relationship);
  struct step_relationship tmp;

  lines_for_each(line, data) {
    int scan_res =
        str_scanf(line, "Step %c must be finished before step %c can begin",
                  &tmp.precondition, &tmp.postcondition);
    if (scan_res < 2) {
      fprintf(stderr, "Failure to parse line: %.*s", string_format_arg(data));
      abort();
    }
    if (not isupper(tmp.precondition)) {
      fprintf(stderr, "precondition is invalid: %c\n", tmp.precondition);
      abort();
    }
    if (not isupper(tmp.postcondition)) {
      fprintf(stderr, "postcondition is invalid: %c\n", tmp.postcondition);
      abort();
    }

    db_push(result, tmp);
  }

  return result;
}

struct step *find_element(struct step *graph, char c) {
  db_for_each(struct step, el, graph) {
    if (el->name == c) {
      return el;
    }
  }

  return NULL;
}

int compare_step(void const *lhs_, void const *rhs_) {
  struct step const *lhs = lhs_;
  struct step const *rhs = rhs_;

  if (lhs->name < rhs->name) {
    return -1;
  } else if (lhs->name > rhs->name) {
    return 1;
  } else {
    return 0;
  }
}

struct step *make_graph(struct step_relationship *step_relationships) {
  struct step *result = db_new(struct step);

  db_for_each(struct step_relationship, rel, step_relationships) {
    struct step *precondition = find_element(result, rel->precondition);
    if (not precondition) {
      struct step tmp;
      tmp.name = rel->precondition;
      tmp.finished = false;
      tmp.preconditions = db_new(char);
      db_push(result, tmp);
    }

    struct step *element = find_element(result, rel->postcondition);
    if (not element) {
      struct step tmp;
      tmp.name = rel->postcondition;
      tmp.finished = false;
      tmp.preconditions = db_new(char);
      db_push(result, tmp);
      element = db_end(result) - 1;
    }

    db_push(element->preconditions, rel->precondition);
  }

  qsort(result, db_length(result), sizeof(*result), compare_step);
  return result;
}

void set_step_done(struct step *graph, char c) {
  db_for_each(struct step, step, graph) {
    char *new_preconditions = db_new(char);
    db_for_each(char, precondition, step->preconditions) {
      if (*precondition != c) {
        db_push(new_preconditions, *precondition);
      }
    }
    db_free(step->preconditions);
    step->preconditions = new_preconditions;
  }
}

void do_the_thing(string data, enum part p) {
  (void)p;
  struct step_relationship *step_relationships = parse_relationships(data);

  struct step *graph = make_graph(step_relationships);
  struct step *finished = db_new(struct step);

  for (;;) {
    struct step *available = NULL;

    // already sorted by alphabet -- chooses the first valid step
    db_for_each(struct step, step, graph) {
      if (db_is_empty(step->preconditions) and not step->finished) {
        printf("pushing element: %c\n", step->name);
        available = step;
        break;
      }
    }

    if (not available) {
      break;
    } else {
      set_step_done(graph, available->name);
      available->finished = true;
      db_push(finished, *available);
    }
  }

  printf("Final order: ");
  db_for_each(struct step, el, finished) { printf("%c", el->name); }
  puts("");
}

DEFINE_MAIN(7)
