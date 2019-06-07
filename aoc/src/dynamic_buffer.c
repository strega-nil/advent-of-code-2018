#include <aoc/dynamic_buffer.h>

#include <stdlib.h>
#include <string.h>

struct dynamic_buffer_repr {
  size_t length;
  size_t capacity;
  unsigned char buffer[];
};

static struct dynamic_buffer_repr*
repr_of(void const* buffer) {
  unsigned char* buffer_ptr = (unsigned char*)buffer;
  return (struct dynamic_buffer_repr*)(
    buffer_ptr - offsetof(struct dynamic_buffer_repr, buffer));
}

size_t _Aoc_dynamic_buffer_length(void const* self) {
  return repr_of(self)->length;
}

size_t _Aoc_dynamic_buffer_capacity(void const* self) {
  return repr_of(self)->capacity;
}

void _Aoc_dynamic_buffer_set_length(void* self, size_t length) {
  repr_of(self)->length = length;
}
void _Aoc_dynamic_buffer_increment_length(void* self) {
  ++repr_of(self)->length;
}
void _Aoc_dynamic_buffer_decrement_length(void* self) {
  --repr_of(self)->length;
}

static struct dynamic_buffer_repr*
internal_with_capacity(size_t el_size, size_t capacity) {
  struct dynamic_buffer_repr* ret =
    malloc(sizeof(*ret) + el_size * capacity);

  ret->capacity = capacity;
  ret->length = 0;

  return ret;
}

void* _Aoc_dynamic_buffer_with_capacity(
  size_t el_size,
  size_t capacity
) {
  return &internal_with_capacity(el_size, capacity)->buffer;
}

void* _Aoc_dynamic_buffer_resize(
  void* self,
  size_t el_size,
  size_t capacity
) {
  struct dynamic_buffer_repr* original = repr_of(self);
  struct dynamic_buffer_repr* ret =
    internal_with_capacity(el_size, capacity);

  size_t length = original->length;
  if (capacity < length) {
    length = capacity;
  }

  memcpy(&ret->buffer, &original->buffer, el_size * length);
  ret->length = length;

  free(original);
  return &ret->buffer;
}

void* _Aoc_dynamic_buffer_resize_if_at_capacity(
  void* self,
  size_t el_size
) {
  struct dynamic_buffer_repr* original = repr_of(self);

  if (original->length == original->capacity) {
    return _Aoc_dynamic_buffer_resize(
      self,
      el_size,
      original->capacity * 2
    );
  } else {
    return self;
  }
}

void _Aoc_dynamic_buffer_free(void* self) {
  free(repr_of(self));
}
