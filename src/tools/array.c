#include "tools/array.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

Array* array__init(size_t elem_size) {
  assert(elem_size > 0);

  Array* arr = malloc(1 * sizeof(Array));
  arr->curr_size = 0;
  arr->max_size = ARRAY_INITIAL_SIZE;
  arr->elem_size = elem_size;
  arr->elements = calloc(ARRAY_INITIAL_SIZE , elem_size);

  for (size_t i=0; i<ARRAY_INITIAL_SIZE; i++) {
    arr->elements[i] = NULL;
  }
  return arr;
}

void array__append(Array* arr, void* elem) {

  assert(sizeof(elem) == arr->elem_size);

  if (arr->curr_size + 1 == arr->max_size) {
    arr->max_size *= 2;
    arr->elements = realloc(arr->elements, 2 * arr->max_size * arr->elem_size);
  }

  arr->elements[arr->curr_size] = elem;
  arr->curr_size++;
}

