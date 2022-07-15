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
  expr->as.binary.left = left;
  expr->as.binary.right = right;
  expr->as.binary.op = op;
  expr->fileloc = fileloc_range(3, left->fileloc, op->fileloc, right->fileloc);
  return expr;
}

Expr* grouping_init(Expr *expression) {
  assert(expression != NULL);

  FileLoc* fl = expression->fileloc;
  Expr* new_expr = malloc(1 * sizeof(Expr));

  new_expr->type = EXPR_GROUPING;
  new_expr->as.grouping.expression = expression;
  new_expr->fileloc = fileloc_init(fl->line, fl->start - 1, fl->end + 1); // handle the parentheses
  return new_expr;
};

Expr* literal_init(LoxObject* value) {
  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_LITERAL;
  expr->as.literal.value = value;
  expr->fileloc = value->fl;
  return expr;
};

Expr* unary_init(Token* op, Expr* right) {
  assert(right != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_UNARY;
  expr->as.unary.op = op;
  expr->as.unary.right = right;
  expr->fileloc = fileloc_range(2, op->fileloc, right->fileloc);
  return expr;
}

Expr* vardecl_init(Token* name) {
  assert(name != NULL);
  Expr* expr = malloc(1 * sizeof(Expr));
  expr->type = EXPR_VAR;
  expr->as.var.name = name;
  expr->fileloc = name->fileloc;
  return expr;
}

Expr* assign_init(Token* name, Expr* value) {
  assert(name != NULL);
  assert(value != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));
  expr->type = EXPR_ASSIGN;
  expr->fileloc = fileloc_range(2, name->fileloc, value->fileloc);

  expr->as.assign.name = name;
  expr->as.assign.value = value;

  return expr;
}
