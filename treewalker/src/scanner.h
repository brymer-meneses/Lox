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
  char *source;

  Token* tokens;
  size_t parsed;
  size_t capacity;

} Scanner;

Scanner scanner_create(char *source);

char scanner_advance(Scanner *scanner);
Token* scanner_scan(Scanner *scanner);
void scanner_scan_token(Scanner *scanner);
void scanner_register_token(Scanner *scanner, Token token);

char _peek(Scanner *scanner);
bool _match(Scanner *scanner, char expected);

#endif
