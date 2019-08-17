#ifndef AOC_UTILITY_H
#define AOC_UTILITY_H

#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef int compare_t(void const*, void const*);

typedef void for_each_t(void* element, void* thunk);

enum part {
	PART_A,
	PART_B,
};

#define DEFINE_MAIN(number) \
	void (*day_ ## number)(string, enum part) = do_the_thing;

#define array_size(self) (sizeof(self) / sizeof(self[0]))

#define range_for_each(type, name, start, end)                         \
	for (type name = (start), __##name##_end = (end);                    \
			 name < __##name##_end;                                          \
			 ++name)

#define slice_for_each(type, name, start, end)                         \
	for (type* name = (start), *__##name##_end = (end);                  \
			 name != __##name##_end;                                         \
			 ++name)

#define array_for_each(type, name, self)                               \
	slice_for_each(type, name, self, &(self)[array_size(self)])

#endif // AOC_UTILITY_H
