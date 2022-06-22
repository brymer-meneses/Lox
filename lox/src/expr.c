#include "lox/expr.h"

Expr expr_init(ExprType type,  void* value, Expr *left, Token *op, Expr *right) {
  Expr expr = {
    .type = type,
    .left = left,
    .op = op,
    .right = right,
    .value = value,
  };
  return expr;
}

Expr binary(Expr *left, Token *op, Expr *right) {
  return expr_init(EXPR_BINARY, NULL, left, op, right);
}

Expr grouping(Expr *expr) {
  return expr_init(EXPR_GROUPING, NULL, expr, NULL, NULL);
};

Expr literal(void* value) {
  return expr_init(EXPR_LITERAL, value, NULL, NULL, NULL);
};

Expr unary(Token* op, Expr* right) {
  return expr_init(EXPR_UNARY, NULL, NULL, NULL, right);
}

void expr_evaluate(Expr *expr) {
  switch (expr->type) {
    case EXPR_BINARY:
    case EXPR_UNARY:
    case EXPR_GROUPING:
    case EXPR_LITERAL:
      break;
  }
  return;
}
