#ifndef TOKEN_H
#define TOKEN_H

#include "tools/fileloc.h"
#include "lox/object.h"
#include "stdlib.h"
#include "stdbool.h"

typedef enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, POW,

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

  SOURCE_END
} TokenType;

typedef struct Token {
  TokenType type;
  size_t line;
  char* lexeme;
  LoxObject* literal;
  FileLoc* fileloc;
} Token;


void  token_print(Token* token);
Token* token_init(TokenType type, char* lexeme, LoxObject* literal, FileLoc* fileloc);
char* tokentype_to_string(TokenType type);

char*  token_to_string(Token token);

#endif
