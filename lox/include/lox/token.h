#ifndef TOKEN_H
#define TOKEN_H

#include "stdlib.h"
#include "declarations.h"
#include "literal.h"

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
  Literal literal;
  size_t line;
  char *lexeme;
} Token;

void token_print(Token token);
Token token_init(TokenType type, char* lexeme, Literal literal, unsigned int line);
char *token_to_string(TokenType type);

#endif
