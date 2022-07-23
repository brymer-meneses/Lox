#ifndef EXPR_H
#define EXPR_H

#include "lox/core.h"


Expr* binary__init(Expr* left, Token* op, Expr *right);
Expr* grouping__init(Expr* expr);
Expr* literal__init(LoxObject* value);
Expr* unary__init(Token* op, Expr* right);
Expr* var__init(Token* name);
Expr* assign__init(Token* name, Expr* value);
Expr* logical__init(Expr* left, Token* op, Expr* right);
Expr* call__init(Expr* callee, Token* paren, unsigned int num_args, Expr** args);

void expr__free(Expr* expr);
#endif
