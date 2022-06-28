#include "stdio.h"
#include "stdlib.h"
#include "lox/state.h"
#include "lox/error.h"

void report(int line, int column, char* message) {
  fprintf(stderr, "[Error line: %d col: %d] %s\n", line, column, message);
  LOX_HAD_ERROR = true;
}


