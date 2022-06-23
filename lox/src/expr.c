#include "lox/expr.h"
#include "stdio.h"

Expr* expr_init(ExprType type,  Literal value, Expr *left, Token op, Expr *right) {
  Expr* expr = malloc(1 * sizeof(Expr));
  expr->type = type;
  expr->left = left;
  expr->op = op;
  expr->right = right;
  expr->value = value;
  return expr;
}

Expr* binary(Expr *left, Token op, Expr *right) {
  return expr_init(EXPR_BINARY, NULL, left, op, right);
}

Expr* grouping(Expr *expr) {
  return expr_init(EXPR_GROUPING, NULL, expr, TOKEN_NULL, NULL);
};

Expr* literal(Literal value) {
  return expr_init(EXPR_LITERAL, value, NULL, TOKEN_NULL, NULL);
};

Expr* unary(Token op, Expr* right) {
  return expr_init(EXPR_UNARY, NULL, NULL, op, right);
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
