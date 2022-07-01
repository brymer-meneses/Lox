#ifndef TOKEN_H
#define TOKEN_H

#include "lox/filelocation.h"
#include "stdlib.h"
#include "stdbool.h"
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

  SOURCE_END
} TokenType;


typedef struct Token {
  TokenType type;
  size_t line;
  const char *lexeme;
  FileLoc fileloc;
} Token;


void  token_print(Token token);
Token token_init(TokenType type, const char* lexeme, const FileLoc fileloc);
char* tokentype_to_string(TokenType type);

double token_parse_double(Token token);
bool   token_parse_bool  (Token token);
char*  token_parse_string(Token token);
char*  token_to_string(Token token);

#endif
