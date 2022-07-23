#ifndef SCANNER_H
#define SCANNER_H

#include "stdlib.h"
#include "stdbool.h"
#include "lox/core.h"

Scanner* scanner__init(char *source);
Token** scanner__scan();

#endif
