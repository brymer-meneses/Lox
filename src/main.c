#include "stdio.h"

#include "lox/lox.h"
#include "lox/error.h"
#include "stdlib.h"


int main(int argc, char* argv[]) {
  lox__init();

  if (argc == 2) {
    lox__run_file(argv[1]);
  } else if (argc == 1) {
    lox__run_prompt();
  } else {
    printf("Usage clox <filename>");
  }

  lox__free();
}
