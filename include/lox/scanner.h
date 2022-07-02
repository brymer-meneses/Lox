#ifndef SCANNER_H
#define SCANNER_H

#include "stdlib.h"
#include "declarations.h"
#include "stdbool.h"

void scanner_init(const char *source);
Token* scanner_scan();

FileLoc compute_relative_position();

#endif
