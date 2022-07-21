#ifndef ARRAY_H
#define ARRAY_H

#include "stdlib.h"
#define ARRAY_INITIAL_SIZE 8

typedef struct Array {
  void** elements;
  size_t elem_size;
  size_t max_size;
  size_t curr_size;
} Array;

Array* array__init(size_t size);
void   array__append(Array* arr, void* elem);


#endif // !ARRAY_H
