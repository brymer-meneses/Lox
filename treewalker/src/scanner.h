#ifndef SCANNER_H
#define SCANNER_H

#include "stdlib.h"
#include "token.h"
#include "stdbool.h"

typedef struct Scanner {
  size_t current;
  size_t start;
  size_t line;
  char *source;

  Token* tokens;
  size_t parsed;
  size_t capacity;

} Scanner;

Scanner scanner_create(char *source);

char scanner_advance(Scanner *scanner);
void scanner_scan_tokens(Scanner *scanner);
void scanner_register_token(Scanner *scanner, Token token);

#endif
