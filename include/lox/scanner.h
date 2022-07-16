#ifndef SCANNER_H
#define SCANNER_H

#include "stdlib.h"
#include "declarations.h"
#include "stdbool.h"
#include "tools/hashmap.h"

typedef struct Scanner {
  // points to the next character
  size_t current;
  // points to the start of a prospective token
  size_t start;
  size_t line;
  char *source;

  Token** tokens;
  size_t parsed;
  size_t capacity;

  // position of the last line character
  size_t last_line;
} Scanner;


Scanner* scanner_init(char *source);
Token** scanner_scan();

#endif
