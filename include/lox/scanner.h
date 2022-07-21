#ifndef SCANNER_H
#define SCANNER_H

#include "stdlib.h"
#include "declarations.h"
#include "stdbool.h"
#include "tools/hashmap.h"
#include "tools/array.h"

typedef struct Scanner {
  // points to the next character
  size_t current;
  // points to the start of a prospective token
  size_t start;
  size_t line;
  char *source;

  Array* tokens_array;
  // position of the last line character
  size_t last_line;
} Scanner;


Scanner* scanner__init(char *source);
Token** scanner__scan();

#endif
