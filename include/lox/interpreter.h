#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include "declarations.h"
#include "stmt.h"
#include "environment.h"


void interpret(Stmt** statements, Environment* env, size_t num_stmts);

#endif
