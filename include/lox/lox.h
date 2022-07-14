#ifndef LOX_H
#define LOX_H

#include "declarations.h"
#include "scanner.h"
#include "parser.h"
#include "environment.h"
  
void run(char source[]);
void run_file(const char* filename);
void run_prompt();
void lox_init();

typedef struct Lox {
  Scanner scanner;
  Parser parser;
  Environment environment;

  bool had_error;
  bool had_runtime_error;

  bool is_on_repl;
} Lox;


extern Lox lox; 

#endif // !LOX_H
