#include "expr.h"

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
  return expr_init(Binary, NULL, left, op, right);
}

Expr grouping(Expr *expr) {
  return expr_init(Grouping, NULL, expr, NULL, NULL);
};

Expr literal(void* value) {
  return expr_init(Literal, value, NULL, NULL, NULL);
};

Expr unary(Token* op, Expr* right) {
  return expr_init(Binary, NULL, NULL, NULL, right);
}

void expr_evaluate(Expr *expr) {
  switch (expr->type) {
    case Binary:
    case Unary:
    case Grouping:
    case Literal:
      break;
  }
  return;
}
