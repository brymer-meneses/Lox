#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "lox/scanner.h"
#include "lox/parser.h"
#include "stdarg.h"

void report(FileLoc* fl, char* message, ...);
void report_two(char* message, ...);
#endif
