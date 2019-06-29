#ifndef AOC_DYNAMIC_BUFFER_H
#define AOC_DYNAMIC_BUFFER_H

#include <aoc/utility.h>

size_t _Aoc_dynamic_buffer_length(void const* self);
size_t _Aoc_dynamic_buffer_capacity(void const* self);

void _Aoc_dynamic_buffer_set_length(void* self, size_t length);
void _Aoc_dynamic_buffer_increment_length(void* self);
void _Aoc_dynamic_buffer_decrement_length(void* self);

void* _Aoc_dynamic_buffer_with_capacity(size_t el_size, size_t capacity);

void* _Aoc_dynamic_buffer_resize(
  void* self,
  size_t el_size,
  size_t new_cap
);
void* _Aoc_dynamic_buffer_resize_if_at_capacity(
  void* self,
  size_t el_size
);

void _Aoc_dynamic_buffer_free(void* self);

#define db_length(self) _Aoc_dynamic_buffer_length(self)
#define db_is_empty(self) (db_length(self) == 0)
#define db_capacity(self) _Aoc_dynamic_buffer_capacity(self)

#define db_start(self) (&(self)[0])
#define db_end(self) (&(self)[db_length(self)])

#define db_new(type) \
  _Aoc_dynamic_buffer_with_capacity(sizeof(type), 8)
#define db_new_with_capacity(type, capacity) \
  _Aoc_dynamic_buffer_with_capacity(sizeof(type), capacity)
#define db_free(self) _Aoc_dynamic_buffer_free(self)

#define db_resize(self, cap) \
  ((self) = _Aoc_dynamic_buffer_resize(self, sizeof(*self), cap))

#define db_push(self, item) \
  ( \
    ((self) = _Aoc_dynamic_buffer_resize_if_at_capacity(self, sizeof(*self))), \
    ((self)[db_length(self)] = (item)), \
    _Aoc_dynamic_buffer_increment_length(self) \
  )

#define db_pop(self) \
  (_Aoc_dynamic_buffer_decrement_length(self), (self)[db_length(self)])

#define db_set_length(self, length) \
  _Aoc_dynamic_buffer_set_length(self, length)

#define db_for_each(type, name, self) \
  slice_for_each(type, name, self, db_end(self))

#endif // AOC_DYNAMIC_BUFFER_H
