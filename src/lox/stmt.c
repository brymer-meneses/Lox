#include "assert.h"
#include "stdlib.h"

#include "lox/stmt.h"
#include "lox/expr.h"

Stmt* stmt__expr_init(Expr* expression) {
  assert(expression != NULL);
  Stmt* stmt = malloc(1 * sizeof(Stmt));

  stmt->type = STMT_EXPRESSION;
  stmt->as.expression.expression = expression;

  return stmt;
}

Stmt* stmt__print_init(Expr* expression) {
  assert(expression != NULL);
  Stmt* stmt = malloc(1 * sizeof(Stmt));

  stmt->type = STMT_PRINT;
  stmt->as.print.expression = expression;

  return stmt;
}

Stmt* stmt__vardecl_init(Token* name, Expr* initializer) {
  assert(name != NULL);
  Stmt* stmt = malloc(1 * sizeof(Stmt));

  stmt->type = STMT_VAR;
  stmt->as.var.initializer = initializer;
  stmt->as.var.name= name;
  return stmt;
}

Stmt* stmt__block_init(size_t length, Stmt** statements) {
  Stmt* stmt = malloc(1 * sizeof(Stmt));
  stmt->type = STMT_BLOCK;
  stmt->as.block.length = length;
  stmt->as.block.statements = statements;
  return stmt;
}

Stmt* stmt__if_init(Expr* condition, Stmt* then_branch, Stmt* else_branch) {
  Stmt* stmt = malloc(1 * sizeof(Stmt));
  stmt->type = STMT_IF;
  stmt->as.if_statement.then_branch = then_branch;
  stmt->as.if_statement.else_branch = else_branch;
  stmt->as.if_statement.condition = condition;
  return stmt;
}

Stmt* stmt__while_loop_init(Expr* condition, Stmt* body) {
  Stmt* stmt = malloc(1 * sizeof(Stmt));
  stmt->type = STMT_WHILE_LOOP;
  stmt->as.while_loop.condition = condition;
  stmt->as.while_loop.body = body;
  return stmt;
}

Stmt* stmt__function_init(Token* name, Token** params, unsigned int arity, Stmt* body) {
  assert(name != NULL);
  assert(params != NULL);
  assert(body->type == STMT_BLOCK);

  Stmt* stmt = malloc(1 * sizeof(Stmt));
  stmt->type = STMT_FUNCTION;
  stmt->as.function.body = body;
  stmt->as.function.name = name;
  stmt->as.function.params = params;
  stmt->as.function.arity = arity;
  return stmt;
}


static void stmt__free(Stmt* stmt) {
  if (stmt == NULL) return;

  switch (stmt->type) {
    case STMT_VAR:
      // skip as this statements contains a token that will be freed later
      // token_free(stmt->as.var.name);
      break;
    case STMT_EXPRESSION:
      expr__free(stmt->as.expression.expression);
      break;
    case STMT_PRINT:
      expr__free(stmt->as.print.expression);
      break;
    case STMT_BLOCK:
      for (size_t i=0; i<stmt->as.block.length; i++) {
        stmt__free((Stmt*) stmt->as.block.statements[i]);
      };
      free(stmt->as.block.statements);
      break;
    default:
      break;
  }
  free(stmt);
}

void stmts__free(size_t num_stmts, Stmt** stmts) {
  if (stmts == NULL) return;
  for (size_t i=0; i<num_stmts; i++) {
    stmt__free(stmts[i]);
  }
  free(stmts);
}
