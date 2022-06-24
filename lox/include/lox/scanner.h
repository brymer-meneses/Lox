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

Scanner scanner_init(char *source);

char scanner_advance(Scanner *s);
Token* scanner_scan(Scanner *s);
void scanner_scan_token(Scanner *s);
void scanner_register_token(Scanner *s, Token token);

char scanner_peek(Scanner *s);
bool scanner_match(Scanner *s, char expected);

#endif
