#ifndef TOKEN_H
#define TOKEN_H

#include "tools/fileloc.h"
#include "lox/object.h"
#include "stdlib.h"
#include "stdbool.h"

typedef struct Token {
  TokenType type;
  size_t line;
  char* lexeme;
  LoxObject literal;
  FileLoc fileloc;
} Token;


void  token_print(Token token);
Token token_init(TokenType type, char* lexeme, LoxObject literal, FileLoc fileloc);
char* tokentype_to_string(TokenType type);

double token_parse_double(Token token);
bool   token_parse_bool  (Token token);
char*  token_parse_string(Token token);
char*  token_to_string(Token token);

#endif
