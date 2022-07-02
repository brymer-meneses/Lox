#ifndef DEBUG_H
#define DEBUG_H

#include "lox/token.h"
#include "lox/expr.h"

void log_tokens(Token* tokens);
void log_ast(Expr* ast);
#endif // !DEBUG_H
