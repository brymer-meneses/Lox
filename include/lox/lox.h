#ifndef LOX_H
#define LOX_H

#include "declarations.h"
#include "lox/interpreter.h"
#include "scanner.h"
#include "parser.h"
#include "environment.h"
  

typedef struct Lox {
  struct {
    bool had_error;
    bool had_runtime_error;
    bool is_on_repl;
  } status;

  char* source_code;
  Interpreter* interpreter;
} Lox;

extern Lox* lox;

void lox__init();
void lox__run(char* source);
void lox__free();

void lox__run_file(const char* filename);
void lox__run_prompt();

#endif // !LOX_H
