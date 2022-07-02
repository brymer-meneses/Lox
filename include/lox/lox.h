#ifndef LOX_H
#define LOX_H

void run(char source[]);
void run_file(const char* filename);
void run_prompt();
void lox_init();

#include "declarations.h"

extern Lox lox; 

#endif // !LOX_H
