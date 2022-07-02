#ifndef LOX_H
#define LOX_H

void run(char source[]);
void run_file(const char* filename);
void run_prompt();

#include "lox/scanner.h"
#include "lox/parser.h"

typedef struct Lox {
  Scanner scanner;
  Parser parser;

  bool had_error;
} Lox;

extern Lox lox; 

#endif // !LOX_H
