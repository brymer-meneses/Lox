#include "assert.h"
#include "stdlib.h"

#include "lox/stmt.h"
#include "lox/expr.h"

Stmt* stmt_expr_init(Expr* expression) {
  assert(expression != NULL);
  Stmt* stmt = malloc(1 * sizeof(Stmt));

  stmt->type = STMT_EXPRESSION;
  stmt->data.Expression.expression = expression;

  return stmt;
}

Stmt* stmt_print_init(Expr* expression) {
  assert(expression != NULL);
  Stmt* stmt = malloc(1 * sizeof(Stmt));

  stmt->type = STMT_PRINT;
  stmt->data.Print.expression = expression;

  return stmt;
}

Stmt* stmt_vardecl_init(Token* name, Expr* initializer) {
  assert(name != NULL);
  Stmt* stmt = malloc(1 * sizeof(Stmt));

  stmt->type = STMT_VAR;
  stmt->data.Var.initializer = initializer;
  stmt->data.Var.name= name;
  return stmt;
}
