#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include "environment.h"
#include "parser.h"
#include "scanner.h"

typedef struct Interpreter {
  Parser* parser;
  Scanner* scanner;
  Environment* globals;
} Interpreter;

Interpreter* interpreter__init();
void interpreter__run(const char* source);
void interpreter__free();

void interpreter__execute(Environment* env, Stmt* stmt);

#endif
