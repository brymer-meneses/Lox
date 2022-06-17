#ifndef SCANNER_H
#define SCANNER_H

#include "stdlib.h"
#include "token.h"

typedef struct Scanner {
  size_t current;
  size_t start;
  size_t line;
  char *source;
  TokenArray tokens;
} Scanner;

Scanner scanner_create(char *source);
void scanner_scan(Scanner *scanner);

#endif
