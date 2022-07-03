#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "declarations.h"
#include "expr.h"

typedef struct Parser {
  const Token* tokens;
  const char* source;
  unsigned int current;
} Parser;



void parser_init(const Token* tokens);
Expr* parser_parse();

#endif 
