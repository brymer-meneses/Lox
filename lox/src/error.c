#include "stdio.h"
#include "stdlib.h"

void report(int line, int column, char* message) {
  fprintf(stderr, "[Error line: %d col: %d] %s\n", line, column, message);
}

void panic(int line, int column, char* message) {
  report(line, column, message);
  exit(1);
}


