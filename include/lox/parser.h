#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "expr.h"
#include "stmt.h"

typedef struct Parser {
  const char* source;
  Token** tokens;
  size_t current;
  size_t num_stmts;
  Stmt** stmts;
  bool need_repl_resolution;
} Parser;

Parser* parser__init(Token** tokens);
Stmt** parser__parse();

#endif 
