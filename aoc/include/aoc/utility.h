#ifndef AOC_UTILITY_H
#define AOC_UTILITY_H

#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>

typedef int compare_t(void const*, void const*);

typedef void for_each_t(void* element, void* thunk);


#endif // AOC_UTILITY_H
