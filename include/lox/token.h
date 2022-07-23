#ifndef TOKEN_H
#define TOKEN_H

#include "lox/core.h"
#include "stdlib.h"
#include "stdbool.h"



void  token__print(Token* token);
Token* token__init(TokenType type, char* lexeme, LoxObject* literal, FileLoc* fileloc);
char* tokentype__to_string(TokenType type);

char*  token__to_string(Token token);
void token__free(Token* token);
void tokens__free(size_t num_tokens, Token** tokens);

#endif
