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
  size_t line;
  char *lexeme;
} Token;


Token token_create(TokenType type, char* lexeme, void *literal);
char *token_to_string(TokenType type);

#endif
