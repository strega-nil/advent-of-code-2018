#include <aoc/dynamic_buffer.h>
#include <aoc/set.h>
#include <aoc/string.h>
#include <aoc/read.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum data_entry_kind {
  ENTRY_INVALID = -1,
  ENTRY_START,
  ENTRY_WAKE,
  ENTRY_SLEEP,
};

// holds {guard_id = -1} if {kind != ENTRY_START}
struct data_entry {
  enum data_entry_kind kind;
  int guard_id;

  // year = 1518 always
  int month;
  int day;
  int hour;
  int minute;
};

struct guard {
  int guard_id;
  int minutes_asleep;
  int count_of_minutes_asleep[60];
};

static int int_compare(void const* lhs_, void const* rhs_) {
  int const* lhs = lhs_;
  int const* rhs = rhs_;

  if (*lhs < *rhs) {
    return -1;
  } else if (*lhs > *rhs) {
    return 1;
  } else {
    return 0;
  }
}

static int data_entry_compare(void const* lhs_, void const* rhs_) {
  struct data_entry const* lhs = lhs_;
  struct data_entry const* rhs = rhs_;

  int compare;

  compare = int_compare(&lhs->month, &rhs->month);
  if (compare != 0) return compare;
  compare = int_compare(&lhs->day, &rhs->day);
  if (compare != 0) return compare;
  compare = int_compare(&lhs->hour, &rhs->hour);
  if (compare != 0) return compare;
  compare = int_compare(&lhs->minute, &rhs->minute);
  return compare;
}

struct data_entry parse_data_entry(string line) {
  struct data_entry ret = {0};
  ret.kind = ENTRY_INVALID;
  ret.guard_id = -1;

  char buffer[64] = {0};

  if (line.start == line.end) {
    fprintf(stderr, "empty string\n");
    return ret;
  }
  if (string_length(line) > 63) {
    fprintf(stderr, "string is too long: %zu\n", string_length(line));
    return ret;
  }

  struct string_split sp = string_split_at(line, ']');

  memcpy(buffer, sp.first.start, string_length(sp.first));
  // already zero-terminated

  int scanf_result = sscanf(
    buffer,
    "[1518-%d-%d %d:%d",
    &ret.month,
    &ret.day,
    &ret.hour,
    &ret.minute
  );

  if (scanf_result < 3) {
    fprintf(
      stderr,
      "invalid date format: %.*s\n",
      (int)string_length(sp.first),
      sp.first.start
    );
    return ret;
  }

  memcpy(buffer, sp.rest.start, string_length(sp.rest));
  buffer[string_length(sp.rest)] = '\0';

  scanf_result = sscanf(
    buffer,
    " Guard #%d begins shift",
    &ret.guard_id
  );

  if (scanf_result == 1) {
    ret.kind = ENTRY_START;
    return ret;
  }

  if (strcmp(buffer, " wakes up") == 0) {
    ret.kind = ENTRY_WAKE;
    return ret;
  }
  if (strcmp(buffer, " falls asleep") == 0) {
    ret.kind = ENTRY_SLEEP;
    return ret;
  }

  fprintf(
    stderr,
    "invalid entry format: %.*s",
    (int)string_length(sp.rest),
    sp.rest.start
  );
  return ret;
}

static int guard_compare(void const* lhs_, void const* rhs_) {
  struct guard const* lhs = lhs_;
  struct guard const* rhs = rhs_;

  return int_compare(&lhs->guard_id, &rhs->guard_id);
}

struct data_entry* get_entries(string data) {
  struct data_entry* entries = db_new(struct data_entry);

  lines_for_each(it, data) {
    struct data_entry entry = parse_data_entry(it);

    if (entry.kind == ENTRY_INVALID) {
      continue;
    }
    db_push(entries, entry);
  }

  qsort(
    entries,
    db_length(entries),
    sizeof(*entries),
    data_entry_compare
  );

  return entries;
}

static void slept_most_total(void* current_, void* max_) {
  struct guard* current = current_;
  struct guard* max = max_;

  if (current->minutes_asleep > max->minutes_asleep) {
    *max = *current;
  }
}

static void slept_most_on_minute(void* current_, void* max_) {
  struct guard* current = current_;
  struct guard* max = max_;

  int old_max = 0;
  int new_max = 0;

  range_for_each(int, i, 0, 60) {
    if (max->count_of_minutes_asleep[i] > old_max) {
      old_max = max->count_of_minutes_asleep[i];
    }
    if (current->count_of_minutes_asleep[i] > new_max) {
      new_max = current->count_of_minutes_asleep[i];
    }
  }

  if (new_max > old_max) {
    *max = *current;
  }
}

struct guard sleepiest_guard(string data, enum part part) {
  struct data_entry* entries = get_entries(data);
  set guards = set_new(guard_compare);

  {
    struct guard tmp_guard = {0};

    struct guard* guard = NULL;
    int sleep_time = -1;
    db_for_each(struct data_entry const, it, entries) {
      if (it->kind == ENTRY_START) {
        tmp_guard.guard_id = it->guard_id;
        guard = set_insert(guards, tmp_guard);
        if (not guard) {
          guard = set_search(guards, tmp_guard);
        }
      } else if (it->kind == ENTRY_SLEEP) {
        if (sleep_time != -1) {
          fprintf(stderr, "sleep twice in a row\n");
          exit(1);
        }

        sleep_time = it->minute;
      } else if (it->kind == ENTRY_WAKE) {
        if (sleep_time == -1) {
          fprintf(stderr, "wake twice in a row\n");
          exit(1);
        }

        guard->minutes_asleep += it->minute - sleep_time;

        range_for_each(int, i, sleep_time, it->minute) {
          ++guard->count_of_minutes_asleep[i];
        }
        sleep_time = -1;
      } else {
        fprintf(stderr, "invalid entry\n");
        exit(1);
      }
    }

    if (sleep_time != -1) {
      fprintf(stderr, "slept without waking up\n");
      exit(1);
    }
  }

  db_free(entries);

  struct guard max = {0};
  switch (part) {
    case PART_A:
      set_for_each(guards, slept_most_total, &max);
      break;
    case PART_B:
      set_for_each(guards, slept_most_on_minute, &max);
      break;
  }

  set_free(guards);

  return max;
}

void do_the_thing(string data, enum part part) {
  struct guard guard = sleepiest_guard(data, part);

  printf("The sleepiest guard is #%d\n", guard.guard_id);

  {
    int minute = 0;
    int number_of_sleeps = 0;

    range_for_each(size_t, idx, 0,  60) {
      if (guard.count_of_minutes_asleep[idx] > number_of_sleeps) {
        minute = (int)idx;
        number_of_sleeps = guard.count_of_minutes_asleep[idx];
      }
    }

    printf("They slept most on minute %d\n", minute);
    printf("The answer is %d\n", minute * guard.guard_id);
  }
}

DEFINE_MAIN(4)
