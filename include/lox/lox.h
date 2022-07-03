#ifndef LOX_H
#define LOX_H

#include "declarations.h"
#include "scanner.h"
#include "parser.h"
  
void run(char source[]);
void run_file(const char* filename);
void run_prompt();
void lox_init();

typedef struct Lox {
  Scanner scanner;
  Parser parser;

  bool had_error;
  bool had_runtime_error;
} Lox;


extern Lox lox; 

#endif // !LOX_H
