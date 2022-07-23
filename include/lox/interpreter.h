#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include "lox/core.h"

Interpreter* interpreter__init();
Environment* interpreter__get_globals();

void interpreter__run(const char* source);
void interpreter__free();
void interpreter__execute(Environment* env, Stmt* stmt);

#endif
