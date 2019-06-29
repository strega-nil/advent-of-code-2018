#include <aoc/dynamic_buffer.h>
#include <aoc/set.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  struct node* parent;
  struct node* left;
  struct node* right;

  int height;
  unsigned char element[];
};

set _Aoc_set_new(compare_t* cmp) {
  set ret = {0};
  ret.cmp = cmp;
  return ret;
}

// returns the height of a node, or -1 if its null
#define HEIGHT(n) ((n) ? (n)->height : -1)

// returns the height of the node, from its children's heights
static int calculate_height(struct node const* n) {
  int left_height = HEIGHT(n->left);
  int right_height = HEIGHT(n->right);
  if (left_height > right_height) {
    return 1 + left_height;
  } else {
    return 1 + right_height;
  }
}

static void left_rotate(set* s, struct node* x) {
  struct node* parent = x->parent;
  struct node* y = x->right;
  struct node* mid = y->left;

  // swap y into place under parent
  y->parent = parent;
  if (not parent) {
    // x is the root
    s->root = y;
  } else if (parent->left == x) {
    parent->left = y;
  } else {
    parent->right = y;
  }

  // put x into place under y
  y->left = x;
  x->parent = y;

  // put mid into place. x.left and y.right are already correct
  x->right = mid;
  if (mid) {
    mid->parent = x;
  }

  x->height = calculate_height(x);
  y->height = calculate_height(y);
}

static void right_rotate(set* s, struct node* y) {
  struct node* parent = y->parent;
  struct node* x = y->left;
  struct node* mid = x->right;

  // swap x into place under parent
  x->parent = parent;
  if (not parent) {
    // y is the root
    s->root = x;
  } else if (parent->left == y) {
    parent->left = x;
  } else {
    parent->right = x;
  }

  // put y into place under x
  x->right = y;
  y->parent = x;

  // put mid into place. x.left and y.right are already correct
  y->left = mid;
  if (mid) {
    mid->parent = y;
  }

  y->height = calculate_height(y);
  x->height = calculate_height(x);
}

static int balance_of(struct node const* n) {
  if (not n) {
    return 0;
  } else {
    return HEIGHT(n->right) - HEIGHT(n->left);
  }
}

static void rebalance(set* s, struct node* n) {
  int balance = balance_of(n);

  if (balance < -1) {
    if (balance_of(n->left) > 0) {
      left_rotate(s, n->left);
    }
    right_rotate(s, n);
  } else if (balance > 1) {
    if (balance_of(n->right) < 0) {
      right_rotate(s, n->right);
    }
    left_rotate(s, n);
  }
}

static struct node* new_node(void const* element, size_t size) {
  struct node* tmp = malloc(sizeof(*tmp) + size);

  tmp->height = 0;
  tmp->parent = NULL;
  tmp->left = NULL;
  tmp->right = NULL;
  memcpy(&tmp->element, element, size);

  return tmp;
}

static void* set_insert_recursive(
  set* self,
  struct node* node,
  void const* element,
  size_t element_size
) {
  // insert - tail recurse on the correct child
  for (;;) {
    int comparison = self->cmp(element, &node->element);

    if (comparison == 0) {
      // we're good! already inserted
      return &node->element;
    } else if (comparison < 0) {
      // element < node->word
      if (not node->left) {
        node->left = new_node(element, element_size);
        node->left->parent = node;
        ++self->size;
        break;
      } else {
        node = node->left;
      }
    } else {
      // w > n.word
      if (not node->right) {
        node->right = new_node(element, element_size);
        node->right->parent = node;
        ++self->size;
        break;
      } else {
        node = node->right;
      }
    }
  }

  // rebalance - tail recurse on node->parent
  // if we've gotten here, then we've inserted and hence return NULL
  for (;;) {
    if (not node) {
      return NULL;
    }
    node->height = calculate_height(node);
    rebalance(self, node);
    node = node->parent;
  }
}

void* _Aoc_set_insert(
  set* self,
  void const* element,
  size_t element_size
) {
  if (not self->root) {
    self->root = new_node(element, element_size);
    return NULL;
  }
  return set_insert_recursive(self, self->root, element, element_size);
}

// TODO: implement removal
#if 0
static bool aoc_set_remove_recursive(
  set* self,
  struct node const* node,
  void const* element
) {
  if (not node) {
    return false;
  }
}

bool _Aoc_set_remove(set* self, void const* element) {
  return aoc_set_remove_recursive(self, self->root, element);
}
#endif

static void* aoc_set_search_recursive(
  set const* self,
  struct node* node,
  void const* element
) {
  // tail recurse on correct child
  for (;;) {
    if (not node) {
      return NULL;
    }

    int compare = self->cmp(element, &node->element);

    if (compare == 0) {
      return &node->element;
    } else if (compare < 0) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
}

void* _Aoc_set_search(set const* self, void const* element) {
  return aoc_set_search_recursive(self, self->root, element);
}

void* _Aoc_set_min(set const* self) {
  struct node* n = self->root;
  if (n == NULL) {
    return NULL;
  }

  for (;;) {
    if (n->left) {
      n = n->left;
    } else {
      return &n->element;
    }
  }
}

void* _Aoc_set_max(set const* self) {
  struct node* n = self->root;
  if (n == NULL) {
    return NULL;
  }

  for (;;) {
    if (n->right) {
      n = n->right;
    } else {
      return &n->element;
    }
  }
}

void _Aoc_set_for_each(
  set const* self,
  for_each_t* f,
  void* thunk
) {
  struct node** node_stack = db_new(struct node*);
  struct node* n = self->root;

  for (;;) {
    while (n) {
      db_push(node_stack, n);
      n = n->left;
    }
    if (db_is_empty(node_stack)) {
      break;
    }

    n = db_pop(node_stack);
    f(&n->element, thunk);
    n = n->right;
  }
}

static void free_subtree(struct node* node) {
  // tail recursion on node->right
  for (;;) {
    if (not node) {
      return;
    }

    free_subtree(node->left);
    struct node* tmp = node;
    node = node->right;
    free(tmp);
  }
}

void _Aoc_set_free(set* self) {
  free_subtree(self->root);
}
