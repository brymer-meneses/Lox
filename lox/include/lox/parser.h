#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "lox/token.h"
#include "lox/expr.h"

typedef struct Parser {
  const Token* tokens;
  const char* source;
  unsigned int current;
} Parser;

Parser parser_init(const Token* tokens, const char* source);
Expr* parser_parse(Parser* p);

#endif 
