#ifndef AOC_SET_H
#define AOC_SET_H

#include <aoc/utility.h>

struct node;

// represented as an AVL-tree
typedef struct set {
  compare_t* cmp;
  struct node* root;
  size_t size;
} set;

set _Aoc_set_new(compare_t* cmp);

void* _Aoc_set_insert(set* self, void const* element, size_t element_size);
// bool _Aoc_set_remove(set* self, void const* element);
void* _Aoc_set_search(set const* self, void const* element);

void* _Aoc_set_min(set const* self);
void* _Aoc_set_max(set const* self);

void _Aoc_set_free(set* self);

void _Aoc_set_for_each(
  set const* self,
  for_each_t* f,
  void* thunk
);

#define set_new(cmp) _Aoc_set_new(cmp)
#define set_insert(self, element) \
  _Aoc_set_insert(&self, &element, sizeof(element))

#define set_search(self, element) _Aoc_set_search(&self, &element)
#define set_min(self) _Aoc_set_min(&self)
#define set_max(self) _Aoc_set_max(&self)

// in-order traversal
#define set_for_each(self, f, thunk) _Aoc_set_for_each(&self, f, thunk)

#define set_free(self) _Aoc_set_free(&self)

#endif // AOC_SET_H
