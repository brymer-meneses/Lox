#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "lox/scanner.h"
#include "lox/parser.h"

void report(char* message, FileLoc* fl);

#endif
