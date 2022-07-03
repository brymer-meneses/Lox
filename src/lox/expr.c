#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

#include "lox/expr.h"
#include "lox/declarations.h"

Expr* binary_init(Expr *left, Token op, Expr *right) {
  assert(left != NULL || right != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_BINARY;
  expr->data.binary.left = left;
  expr->data.binary.right = right;
  expr->data.binary.operation = op;
  return expr;
}

Expr* grouping_init(Expr *expression) {
  assert(expression != NULL);
  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_GROUPING;
  expr->data.grouping.expression = expression;
  return expr;
};

Expr* literal_init(LoxObject value) {
  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_LITERAL;
  expr->data.literal.value = value;
  return expr;
};

Expr* unary_init(Token op, Expr* right) {
  assert(right != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_UNARY;
  expr->data.unary.operation = op;
  expr->data.unary.right = right;
  return expr;
}

