#ifndef SCANNER_H
#define SCANNER_H

#include "stdlib.h"
#include "token.h"
#include "stdbool.h"

typedef struct Scanner {
  // points to the next character
  size_t current;
  // points to the start of a prospective token
  size_t start;
  size_t line;
  const char *source;

  Token* tokens;
  size_t parsed;
  size_t capacity;

  // position of the last line character
  size_t last_line;
} Scanner;

void scanner_init(const char *source);
Token* scanner_scan();

FileLoc compute_relative_position();

#endif
