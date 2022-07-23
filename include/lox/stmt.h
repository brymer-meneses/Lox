#ifndef STMT_H
#define STMT_H

#include "lox/core.h"

Stmt* stmt__expr_init(Expr* expression);
Stmt* stmt__while_init(Expr* condition, Stmt* body);
Stmt* stmt__return_init(Expr* keyword, Expr* value);
Stmt* stmt__print_init(Expr* expression);
Stmt* stmt__block_init(size_t length, Stmt** statements);
Stmt* stmt__function_init(Token* name, Token** params, unsigned int arity, Stmt* body);

Stmt* stmt__vardecl_init(Token* name, Expr* initializer);
Stmt* stmt__if_init(Expr* condition, Stmt* then_branch, Stmt* else_branch);
Stmt* stmt__while_loop_init(Expr* condition, Stmt* body);

void stmts__free(size_t num_stmts, Stmt** stmts);


#endif // !STMT_H
