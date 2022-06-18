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

void scanner_add_token(Scanner *scanner, TokenType type, void* literal);

bool _isfinished(Scanner *scanner);
void _scan_string(Scanner *scanner);
void _scan_number(Scanner *scanner);
char _peek_next(Scanner *scanner);

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
    case '/':
      if (_match(scanner, '/')) {
        while(_peek(scanner) != '\n') scanner_advance(scanner);
      } else {
        scanner_add_token(scanner, SLASH, NULL);
      }
       break;
    
    case '"':
      _scan_string(scanner);
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
        _scan_number(scanner);
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

void scanner_add_token(Scanner *scanner, TokenType type, void* literal) {
  char* text = substring(scanner->source, scanner->start, scanner->current);
  Token token = token_create(type, text, literal);
  scanner_register_token(scanner, token);
}

char scanner_advance(Scanner *scanner) {
  if (_isfinished(scanner)) return '\0';

  scanner->current++;
  return scanner->source[scanner->current - 1];
}

void _scan_number(Scanner *scanner) {

  while (isdigit(_peek(scanner))) 
    scanner_advance(scanner);

  
  if (_peek(scanner) == '.') {
    scanner_advance(scanner);
    while (isdigit(_peek(scanner))) 
      scanner_advance(scanner);
  } 

  // Handle the instance wherein we find another decimal point
  if (_peek(scanner) == '.') {
    panic(scanner->line, scanner->current, "Invalid number, has greater than one decimal point.");
  }

  char* number = substring(scanner->source, scanner->start, scanner->current);
  scanner_add_token(scanner, NUMBER, (void*) number);


}

void _scan_string(Scanner *scanner) {
  while (_peek(scanner) != '"' && !_isfinished(scanner)) {
    if (_peek(scanner) == '\n') scanner->line++;
    scanner_advance(scanner);
  }

  // handle when the string doesn't terminate
  if (_isfinished(scanner)) {
    panic(scanner->line, scanner->current, "Unterminated string.");
    return;
  }

// trim the quotes
  scanner_add_token(scanner, STRING, substring(scanner->source, scanner->start+1, scanner->current-1)); 

  // consume the last '"' character
  scanner_advance(scanner);
}


bool _isfinished(Scanner *scanner) {
  return scanner->current >= strlen(scanner->source);
}

char _peek(Scanner *scanner) {
  if (_isfinished(scanner)) return '\0';
  
  return scanner->source[scanner->current];
}

char _peek_next(Scanner *scanner) {
  if (_isfinished(scanner)) return '\0';
  
  return scanner->source[scanner->current+1];
}

bool _match(Scanner *scanner, char expected) {
  if (_peek(scanner) == expected) {
    scanner->current ++;
    return true;
  }
  return false;
}
