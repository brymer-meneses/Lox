#ifndef EXPR_H
#define EXPR_H

#include "token.h"

struct Visitor;
struct Expr;

typedef enum ExprType {
  Binary,
  Unary,
  Literal,
  Grouping,
} ExprType;


typedef struct Expr {
  const ExprType type;
  const struct Expr *left;
  const struct Expr *right;
  const void *value;
  const Token *op;
} Expr;

  

#endif
