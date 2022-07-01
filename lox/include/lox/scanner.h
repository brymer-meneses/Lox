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

Scanner scanner_init(const char *source);

char scanner_advance(Scanner *s);
Token* scanner_scan(Scanner *s);
void scanner_scan_token(Scanner *s);
void scanner_register_token(Scanner *s, Token token);
char* scanner_get_current_line(const Scanner* s);

char scanner_peek(Scanner *s);
bool scanner_match(Scanner *s, char expected);

#endif
