#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "declarations.h"



Expr* expr_init(ExprType type,  LoxObject value, Expr *left, Token op, Expr *right);

Expr* binary(Expr *left, Token op, Expr *right);
Expr* grouping(Expr *expr);
Expr* literal(LoxObject value);
Expr* unary(Token op, Expr* right);


  

#endif
