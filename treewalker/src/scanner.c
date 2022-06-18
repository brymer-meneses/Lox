#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "token.h"
#include "scanner.h"
#include "strutils.h"
#include "assert.h"

void scanner_add_token(Scanner *scanner, TokenType type, void* literal);
void scanner_classify_token(Scanner* scanner);

bool _isfinished(Scanner *scanner);

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

Token* scanner_scan(Scanner* scanner) {
  while (!_isfinished(scanner)) {
    scanner->start = scanner->current;
    scanner_scan_token(scanner);
  }
  scanner_add_token(scanner, FILE_EOF, NULL);
  return scanner->tokens;
}

void scanner_scan_token(Scanner* scanner) {
  char c = scanner_advance(scanner);
  switch (c) {
    case '{':
      scanner_add_token(scanner, LEFT_BRACE, NULL);
      break;
    case '}': 
      scanner_add_token(scanner, RIGHT_BRACE, NULL);
      break;
    case '(':
      scanner_add_token(scanner, LEFT_PAREN, NULL);
      break;
    case ')':
      scanner_add_token(scanner, RIGHT_PAREN, NULL);
      break;
    case ',':
      scanner_add_token(scanner, COMMA, NULL);
      break;
    case '.':
      scanner_add_token(scanner, DOT, NULL);
      break;
    case '*':
      scanner_add_token(scanner, STAR, NULL);
      break;
    case '+':
      scanner_add_token(scanner, PLUS, NULL);
      break;
    case '-':
      scanner_add_token(scanner, MINUS, NULL);
      break;
    case ';':
      scanner_add_token(scanner, SEMICOLON, NULL);
      break;

    case '>':
      if (_match(scanner, '=')) 
        scanner_add_token(scanner, GREATER_EQUAL, NULL);
      else
        scanner_add_token(scanner, GREATER, NULL);
      break;

    case '<':
      if (_match(scanner, '=')) 
        scanner_add_token(scanner, LESS_EQUAL, NULL);
      else
        scanner_add_token(scanner, LESS, NULL);
      break;

    case '=':
      if (_match(scanner, '=')) 
        scanner_add_token(scanner, EQUAL_EQUAL, NULL);
      else 
        scanner_add_token(scanner, EQUAL, NULL);
      break;

    case '!':
      if (_match(scanner, '='))
        scanner_add_token(scanner, BANG_EQUAL, NULL);
      else 
        scanner_add_token(scanner, BANG, NULL);
      break;
    
    case ' ':
    case '\t':
    case '\r':
      // ignore whitespaces
      break;

    case '\n':
      scanner->line++;
      break;

  }

}

void scanner_register_token(Scanner *scanner, Token token) {
  if (scanner->parsed == scanner->capacity) {
    scanner->capacity = 2 * scanner->capacity;

    scanner->tokens = realloc(scanner->tokens, scanner->capacity * sizeof(Token));
  }

  scanner->tokens[scanner->parsed++] = token;
}

void scanner_add_token(Scanner *scanner, TokenType type, void* literal) {
  char* text = substring(scanner->source, scanner->start, scanner->current);
  Token token = token_create(type, text, literal);
  scanner_register_token(scanner, token);
}

char scanner_advance(Scanner *scanner) {
  if (!_isfinished(scanner)) {
    scanner->current++;
    return scanner->source[scanner->current - 1];
  } 

  return '\0';
}


bool _isfinished(Scanner *scanner) {
  return scanner->current >= strlen(scanner->source);
}

char _peek(Scanner *scanner) {
  if (_isfinished(scanner))
    return '\0';
  
  return scanner->source[scanner->current + 1];
}

bool _match(Scanner *scanner, char expected) {
  printf("peek: %c\n", _peek(scanner));
  // printf("got: %c, expected: %c\n", _peek(scanner), expected);
  if (_peek(scanner) == expected) {
    scanner->current++;
    return true;
  }
  return false;
}
