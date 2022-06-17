#ifndef TOKEN_H
#define TOKEN_H

#include "stdlib.h"
#include "declarations.h"

typedef enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  FILE_EOF
} TokenType;


typedef struct Token {
  TokenType type;
  void *literal;
  unsigned int line;
  char lexeme[100];
} Token;

char *to_string(TokenType type);

typedef struct TokenArray {
  Token *tokens;
  size_t size;
  size_t used;
} TokenArray;


TokenArray token_array_create();
void token_array_append(TokenArray* array, Token token);

#endif
