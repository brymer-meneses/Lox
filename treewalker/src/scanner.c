#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"

#include "token.h"
#include "scanner.h"
#include "strutils.h"
#include "assert.h"
#include "error.h"

void add_token(Scanner *s, TokenType type, void* literal);

bool is_finished(Scanner *s);
void scan_string(Scanner *s);
void scan_number(Scanner *s);
char peek_next(Scanner  *s);

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
  while (!is_finished(scanner)) {
    scanner->start = scanner->current;
    scanner_scan_token(scanner);
  }
  add_token(scanner, FILE_EOF, NULL);
  return scanner->tokens;
}

void scanner_scan_token(Scanner* scanner) {
  char c = advance(scanner);
  switch (c) {
    case '{':
      add_token(scanner, LEFT_BRACE, NULL);
      break;
    case '}': 
      add_token(scanner, RIGHT_BRACE, NULL);
      break;
    case '(':
      add_token(scanner, LEFT_PAREN, NULL);
      break;
    case ')':
      add_token(scanner, RIGHT_PAREN, NULL);
      break;
    case ',':
      add_token(scanner, COMMA, NULL);
      break;
    case '.':
      add_token(scanner, DOT, NULL);
      break;
    case '*':
      add_token(scanner, STAR, NULL);
      break;
    case '+':
      add_token(scanner, PLUS, NULL);
      break;
    case '-':
      add_token(scanner, MINUS, NULL);
      break;
    case ';':
      add_token(scanner, SEMICOLON, NULL);
      break;

    case '>':
      if (match(scanner, '=')) 
        add_token(scanner, GREATER_EQUAL, NULL);
      else
        add_token(scanner, GREATER, NULL);
      break;

    case '<':
      if (match(scanner, '=')) 
        add_token(scanner, LESS_EQUAL, NULL);
      else
        add_token(scanner, LESS, NULL);
      break;

    case '=':
      if (match(scanner, '=')) 
        add_token(scanner, EQUAL_EQUAL, NULL);
      else 
        add_token(scanner, EQUAL, NULL);
      break;

    case '!':
      if (match(scanner, '='))
        add_token(scanner, BANG_EQUAL, NULL);
      else 
        add_token(scanner, BANG, NULL);
      break;
    case '/':
      if (match(scanner, '/')) {
        while(peek(scanner) != '\n') advance(scanner);
      } else {
        add_token(scanner, SLASH, NULL);
      }
       break;
    
    case '"':
      scan_string(scanner);
      break;
    case ' ':
    case '\t':
    case '\r':
      // ignore whitespaces
      break;

    case '\n':
      scanner->line++;
      break;
    default:
      if (isdigit(c)) {
        scan_number(scanner);
      }
      break;
  }

}

void scanner_register_token(Scanner *scanner, Token token) {
  if (scanner->parsed == scanner->capacity) {
    scanner->capacity = 2 * scanner->capacity;

    scanner->tokens = realloc(scanner->tokens, scanner->capacity * sizeof(Token));
  }
  scanner->tokens[scanner->parsed] = token;
  scanner->parsed++;
}

void add_token(Scanner *scanner, TokenType type, void* literal) {
  char* text = substring(scanner->source, scanner->start, scanner->current);
  Token token = token_create(type, text, literal);
  scanner_register_token(scanner, token);
}

char advance(Scanner *scanner) {
  if (is_finished(scanner)) return '\0';

  scanner->current++;
  return scanner->source[scanner->current - 1];
}

void scan_number(Scanner *scanner) {

  while (isdigit(peek(scanner))) 
    advance(scanner);

  
  if (peek(scanner) == '.') {
    advance(scanner);
    while (isdigit(peek(scanner))) 
      advance(scanner);
  } 

  // Handle the instance wherein we find another decimal point
  if (peek(scanner) == '.') {
    panic(scanner->line, scanner->current, "Invalid number, has greater than one decimal point.");
  }

  char* number = substring(scanner->source, scanner->start, scanner->current);
  add_token(scanner, NUMBER, (void*) number);


}

void scan_string(Scanner *scanner) {
  while (peek(scanner) != '"' && !is_finished(scanner)) {
    if (peek(scanner) == '\n') scanner->line++;
    advance(scanner);
  }

  // handle when the string doesn't terminate
  if (is_finished(scanner)) {
    panic(scanner->line, scanner->current, "Unterminated string.");
    return;
  }

  // trim the quotes
  add_token(scanner, STRING, substring(scanner->source, scanner->start+1, scanner->current-1)); 

  // consume the last '"' character
  advance(scanner);
}


bool is_finished(Scanner *scanner) {
  return scanner->current >= strlen(scanner->source);
}

char peek(Scanner *scanner) {
  if (is_finished(scanner)) return '\0';
  
  return scanner->source[scanner->current];
}

char peek_next(Scanner *scanner) {
  if (is_finished(scanner)) return '\0';
  
  return scanner->source[scanner->current+1];
}

bool match(Scanner *scanner, char expected) {
  if (peek(scanner) == expected) {
    scanner->current ++;
    return true;
  }
  return false;
}
