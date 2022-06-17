#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "token.h"
#include "scanner.h"

bool scanner_isfinished(Scanner *scanner);

Scanner scanner_create(char* source) {
  Token *tokens = calloc(INITIAL_TOKEN_ARRAY_SIZE, sizeof(Token));

  Scanner scanner = {
    .current =0, 
    .start =0, 
    .line =1, 
    .source=source,
    .capacity = INITIAL_TOKEN_ARRAY_SIZE,
    .parsed = 0,
    .tokens = tokens,
  };

  return scanner;
}

void scanner_scan_tokens(Scanner* scanner) {
  
}

void scanner_classify_token(Scanner* scanner) {
  char c = scanner_advance(scanner);
  // switch (c) {
  //   case '{': scanner_register_token(scanner, Token token)
  // }

}

void scanner_register_token(Scanner *scanner, Token token) {
  if (scanner->parsed == scanner->capacity) {
    scanner->capacity = 2 * scanner->capacity;

    scanner->tokens = realloc(scanner->tokens, scanner->capacity * sizeof(Token));
  }

  scanner->tokens[scanner->parsed++] = token;
}


char scanner_advance(Scanner *scanner) {
  if (!scanner_isfinished(scanner)) {
    scanner->current++;
    return scanner->source[scanner->current - 1];
  } 

  return '\0';
}


bool scanner_isfinished(Scanner *scanner) {
  return scanner->current >= strlen(scanner->source);
}

char _peek(Scanner *scanner) {
  return scanner->source[scanner->current];
}
