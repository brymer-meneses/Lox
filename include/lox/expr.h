#ifndef EXPR_H
#define EXPR_H

#include "token.h"

struct Visitor;
struct Expr;

typedef enum ExprType {
  EXPR_BINARY,
  EXPR_UNARY,
  EXPR_LITERAL,
  EXPR_GROUPING,
} ExprType;


typedef struct Expr {
  ExprType type;
  struct Expr *left;
  struct Expr *right;
  LoxObject value;
  Token op;
} Expr;

Expr* expr_init(ExprType type,  LoxObject value, Expr *left, Token op, Expr *right);

Expr* binary(Expr *left, Token op, Expr *right);
Expr* grouping(Expr *expr);
Expr* literal(LoxObject value);
Expr* unary(Token op, Expr* right);


  

#endif
