#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "assert.h"

#include "lox/token.h"
#include "lox/scanner.h"
#include "lox/error.h"
#include "lox/utils.h"


void add_token(Scanner *s, TokenType type);

void scan_string(Scanner *s);
void scan_number(Scanner *s);
void scan_identifier(Scanner *s);

bool scanner_isfinished(Scanner *s);
char scanner_peek_next(Scanner  *s);

TokenType get_keyword(const char* lexeme);

Scanner scanner_init(char* source) {
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

Token* scanner_scan(Scanner* s) {
  while (!scanner_isfinished(s)) {
    s->start = s->current;
    scanner_scan_token(s);
  }
  add_token(s, SOURCE_END);
  return s->tokens;
}

void scanner_scan_token(Scanner* s) {
  char c = scanner_advance(s);
  switch (c) {
    case '{':
      add_token(s, LEFT_BRACE);
      break;
    case '}': 
      add_token(s, RIGHT_BRACE);
      break;
    case '(':
      add_token(s, LEFT_PAREN);
      break;
    case ')':
      add_token(s, RIGHT_PAREN);
      break;
    case ',':
      add_token(s, COMMA);
      break;
    case '.':
      add_token(s, DOT);
      break;
    case '*':
      add_token(s, STAR);
      break;
    case '+':
      add_token(s, PLUS);
      break;
    case '-':
      add_token(s, MINUS);
      break;
    case ';':
      add_token(s, SEMICOLON);
      break;

    case '>':
      if (scanner_match(s, '=')) 
        add_token(s, GREATER_EQUAL);
      else
        add_token(s, GREATER);
      break;

    case '<':
      if (scanner_match(s, '=')) 
        add_token(s, LESS_EQUAL);
      else
        add_token(s, LESS);
      break;

    case '=':
      if (scanner_match(s, '=')) 
        add_token(s, EQUAL_EQUAL);
      else 
        add_token(s, EQUAL);
      break;

    case '!':
      if (scanner_match(s, '='))
        add_token(s, BANG_EQUAL);
      else 
        add_token(s, BANG);
      break;
    case '/':
      if (scanner_match(s, '/')) {
        while(scanner_peek(s) != '\n')
          scanner_advance(s);
      } else {
        add_token(s, SLASH);
      }
       break;
    
    case '"':
      scan_string(s);
      break;
    case ' ':
    case '\t':
    case '\r':
      // ignore whitespaces
      break;

    case '\n':
      s->line++;
      break;
    default:
      if (isdigit(c)) {
        scan_number(s);
      } else if (isalpha(c)) {
        scan_identifier(s);
      } else {
        report(s->line, s->current -1,  "ERROR: Unexpected character");
      }
      break;
  }

}

void scanner_register_token(Scanner *s, Token token) {
  if (s->parsed == s->capacity) {
    s->capacity = 2 * s->capacity;

    s->tokens = realloc(s->tokens, s->capacity * sizeof(Token));
  }
  s->tokens[s->parsed] = token;
  s->parsed++;
}

void add_token(Scanner *s, TokenType type) {
  char* text = substring(s->source, s->start, s->current-1); // current points to the next "char" so we subtract by 1
  Token token = token_init(type, text, s->line);
  scanner_register_token(s, token);
}

char scanner_advance(Scanner *s) {
  if (scanner_isfinished(s)) return '\0';

  s->current++;
  return s->source[s->current - 1];
}

void scan_number(Scanner *s) {

  while (isdigit(scanner_peek(s))) 
    scanner_advance(s);

  
  if (scanner_peek(s) == '.') {
    scanner_advance(s);
    while (isdigit(scanner_peek(s))) 
      scanner_advance(s);
  } 

  // Handle the instance wherein we find another decimal point
  if (scanner_peek(s) == '.') {
    panic(s->line, s->current, "Invalid number, has greater than one decimal point.");
  }

  add_token(s, NUMBER);


}

void scan_string(Scanner *s) {
  while (scanner_peek(s) != '"' && !scanner_isfinished(s)) {
    if (scanner_peek(s) == '\n') s->line++;
    scanner_advance(s);
  }

  // handle when the string doesn't terminate
  if (scanner_isfinished(s)) {
    panic(s->line, s->current, "Unterminated string.");
    return;
  }
  // consume the last '"' character
  scanner_advance(s);

  add_token(s, STRING); 
}

void scan_identifier(Scanner *s) {
  while (isalnum(scanner_peek(s)))
    scanner_advance(s);

  char* identifier = substring(s->source, s->start, s->current-1);
  TokenType type = get_keyword(identifier);

  add_token(s, type);
}


bool scanner_isfinished(Scanner *s) {
  return s->current >= strlen(s->source);
}

char scanner_peek(Scanner *s) {
  if (scanner_isfinished(s)) return '\0';
  
  return s->source[s->current];
}

char scanner_peek_next(Scanner *s) {
  if (scanner_isfinished(s)) return '\0';
  
  return s->source[s->current+1];
}

bool scanner_match(Scanner *s, char expected) {
  // printf("Got: %c, Expected: %c \n", scanner_peek(scanner), expected);
  if (scanner_peek(s) == expected) {
    s->current ++;
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


