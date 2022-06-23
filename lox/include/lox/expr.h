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
  const ExprType type;
  const struct Expr *left;
  const struct Expr *right;
  const Literal value;
  const Token *op;
} Expr;

  

#endif
