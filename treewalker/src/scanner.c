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


void add_token(Scanner *scanner, TokenType type, void* literal);

void scan_string(Scanner *scanner);
void scan_number(Scanner *scanner);
void scan_identifier(Scanner *scanner);

bool is_finished(Scanner *scanner);
char peek_next(Scanner  *scanner);

TokenType get_keyword(const char* scanner);

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
      } else if (isalpha(c)) {
        scan_identifier(scanner);
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

  char* number = substring(scanner->source, scanner->start, scanner->current-1);
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

  // consume the last '"' character
  advance(scanner);

  // NOTE:
  // the variable 'current' points to the next character, this is why we subtract by 2.
  //
  // Example: "abcd"\n
  // `current` points to the newline character, this is why we need to subtract by two.
  add_token(scanner, STRING, substring(scanner->source, scanner->start+1, scanner->current-2)); 

}

void scan_identifier(Scanner *scanner) {
  while (isalnum(peek(scanner)))
    advance(scanner);

  char* identifier = substring(scanner->source, scanner->start, scanner->current-1);
  TokenType type = get_keyword(identifier);

  add_token(scanner, type, NULL);
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

TokenType get_keyword(const char* text) {
  struct KeywordPair {
    const char* lexeme;
    const TokenType type;
  };

  const struct KeywordPair pairs[] = {
   {"and", AND},
   {"class", CLASS},
   {"else", ELSE},
   {"false", FALSE},
   {"for", FOR},
   {"fun", FUN},
   {"if", IF},
   {"nil", NIL},
   {"or", OR},
   {"print", PRINT},
   {"return", RETURN},
   {"super", SUPER},
   {"this", THIS},
   {"true", TRUE},
   {"var", VAR},
   {"while", WHILE},
  };

  unsigned int length = sizeof(pairs)/sizeof(struct KeywordPair);

  for (int i=0; i<length; i++) {
    if (strcmp(pairs[i].lexeme, text) == 0) {
      return pairs[i].type;
    }
  }
  return IDENTIFIER;
}


