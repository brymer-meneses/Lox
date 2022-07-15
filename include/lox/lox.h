#ifndef LOX_H
#define LOX_H

#include "declarations.h"
#include "scanner.h"
#include "parser.h"
#include "environment.h"
  

typedef struct Lox {
  struct {
    bool had_error;
    bool had_runtime_error;
    bool is_on_repl;
  } status;

  struct {
    Scanner* scanner;
    Parser* parser;
    Environment* environment;
    char* source_code;
  } context;

} Lox;

Lox* lox__init();
void lox__run(char* source);
Lox* lox__get();
void lox__free();

void lox__run_file(const char* filename);
void lox__run_prompt();

#endif // !LOX_H
