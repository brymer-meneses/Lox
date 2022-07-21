#include "lox/token.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

#include "lox/expr.h"
#include "lox/declarations.h"
#include "lox/stmt.h"
#include "tools/fileloc.h"

Expr* binary__init(Expr* left, Token* op, Expr* right) {
  assert(left != NULL || right != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_BINARY;
  expr->as.binary.left = left;
  expr->as.binary.right = right;
  expr->as.binary.op = op;
  expr->fileloc = fileloc__range(3, left->fileloc, op->fileloc, right->fileloc);
  return expr;
}

Expr* grouping__init(Expr *expression) {
  assert(expression != NULL);

  FileLoc* fl = expression->fileloc;
  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_GROUPING;
  expr->as.grouping.expression = expression;
  expr->fileloc = fileloc__init(fl->line, fl->start - 1, fl->end + 1); // handle the parentheses
  return expr;
};

Expr* literal__init(LoxObject* value) {
  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_LITERAL;
  expr->as.literal.value = value;
  expr->fileloc = value->fl;
  return expr;
};

Expr* unary__init(Token* op, Expr* right) {
  assert(right != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_UNARY;
  expr->as.unary.op = op;
  expr->as.unary.right = right;
  expr->fileloc = fileloc__range(2, op->fileloc, right->fileloc);
  return expr;
}

Expr* var__init(Token* name) {
  assert(name != NULL);
  Expr* expr = malloc(1 * sizeof(Expr));
  expr->type = EXPR_VAR;
  expr->as.var.name = name;
  expr->fileloc = name->fileloc;
  return expr;
}

Expr* assign__init(Token* name, Expr* value) {
  assert(name != NULL);
  assert(value != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));
  expr->type = EXPR_ASSIGN;
  expr->fileloc = fileloc__range(2, name->fileloc, value->fileloc);

  expr->as.assign.name = name;
  expr->as.assign.value = value;

  return expr;
}

Expr* logical__init(Expr* left, Token* op, Expr* right) {
  assert(left != NULL || right != NULL);
  Expr* expr = malloc(1 * sizeof(Expr));

  expr->type = EXPR_LOGICAL;
  expr->as.logical.left = left;
  expr->as.logical.right = right;
  expr->as.logical.op = op;
  expr->fileloc = fileloc__range(3, left->fileloc, op->fileloc, right->fileloc);
  return expr;
}

Expr* call__init(Expr* callee, Token* paren, unsigned int num_args, Expr** args) {
  assert(callee != NULL);
  assert(paren != NULL);

  Expr* expr = malloc(1 * sizeof(Expr));
  expr->type = EXPR_CALL;
  expr->as.call.callee = callee;
  expr->as.call.paren = paren;
  expr->as.call.num_args = num_args;
  expr->as.call.args = args;

  return expr;
}

void expr__free(Expr* expr) {
  if (expr == NULL) return;

  // NOTE: tokens are freed by tokens_free on lox__free, since we only use 1 copy 
  // of them throughout the runtime of the interpreter.

  switch (expr->type) {
    case EXPR_BINARY:
      expr__free(expr->as.binary.left);
      expr__free(expr->as.binary.right);
      break;
    case EXPR_LOGICAL:
      expr__free(expr->as.logical.left);
      expr__free(expr->as.logical.right);
      break;
    case EXPR_GROUPING:
      expr__free(expr->as.grouping.expression);
      break;
    case EXPR_LITERAL:
      loxobject__free(expr->as.literal.value);
      break;
    case EXPR_UNARY:
      expr__free(expr->as.unary.right);
      break;
    case EXPR_VAR:
      break;
    case EXPR_ASSIGN:
      expr__free(expr->as.assign.value);
      break;
  }
}
