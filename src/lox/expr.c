#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

#include "lox/expr.h"
#include "lox/declarations.h"
#include "tools/fileloc.h"

Expr* binary_init(Expr* left, Token* op, Expr* right) {
  assert(left != NULL || right != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_BINARY;
  expr->data.binary.left = left;
  expr->data.binary.right = right;
  expr->data.binary.operation = op;
  expr->fileloc = fileloc_range(3, left->fileloc, op->fileloc, right->fileloc);
  return expr;
}

Expr* grouping_init(Expr *expression) {
  assert(expression != NULL);

  FileLoc* fl = expression->fileloc;
  Expr* new_expr = malloc(1 * sizeof(Expr));

  new_expr->type = EXPR_GROUPING;
  new_expr->data.grouping.expression = expression;
  new_expr->fileloc = fileloc_init(fl->line, fl->start - 1, fl->end + 1); // handle the parentheses
  return new_expr;
};

Expr* literal_init(LoxObject* value) {
  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_LITERAL;
  expr->data.literal.value = value;
  expr->fileloc = value->fl;
  return expr;
};

Expr* unary_init(Token* op, Expr* right) {
  assert(right != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_UNARY;
  expr->data.unary.operation = op;
  expr->data.unary.right = right;
  expr->fileloc = fileloc_range(2, op->fileloc, right->fileloc);
  return expr;
}

