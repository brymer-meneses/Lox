#include "stdio.h"

#include "lox/expr.h"
#include "lox/declarations.h"

Expr* expr_init(ExprType type,  LoxObject value, Expr *left, Token op, Expr *right) {
  Expr* expr = malloc(1 * sizeof(Expr));
  expr->type = type;
  expr->left = left;
  expr->op = op;
  expr->right = right;
  expr->value = value;
  return expr;
}

Expr* binary(Expr *left, Token op, Expr *right) {
  return expr_init(EXPR_BINARY, LOX_OBJECT_NULL, left, op, right);
}

Expr* grouping(Expr *expr) {
  return expr_init(EXPR_GROUPING, LOX_OBJECT_NULL, expr, TOKEN_NULL, NULL);
};

Expr* literal(LoxObject value) {
  return expr_init(EXPR_LITERAL, value, NULL, TOKEN_NULL, NULL);
};

Expr* unary(Token op, Expr* right) {
  return expr_init(EXPR_UNARY, LOX_OBJECT_NULL, NULL, op, right);
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
