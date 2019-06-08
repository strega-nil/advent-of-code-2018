#ifndef AOC_SET_H
#define AOC_SET_H

#include <aoc/utility.h>

typedef int compare_t(void const*, void const*);

struct node;

// represented as an AVL-tree
typedef struct set {
  compare_t* cmp;
  struct node* root;
} set;

set _Aoc_set_new(compare_t* cmp);

void* _Aoc_set_insert(set* self, void const* element, size_t element_size);
// bool _Aoc_set_remove(set* self, void const* element);
void* _Aoc_set_search(set const* self, void const* element);

void _Aoc_set_free(set* self);

#define set_new(cmp) _Aoc_set_new(cmp)
#define set_insert(self, element) \
  _Aoc_set_insert(&self, &element, sizeof(element))
#define set_search(self, element) _Aoc_set_search(&self, &element)

#define set_free(self) _Aoc_set_free(&self)

#endif // AOC_SET_H
