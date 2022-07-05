#include "stdio.h"

#include "lox/lox.h"
#include "tools/hashtable.h"

int main(int argc, char *argv[]) {

  if (argc == 2) {
    run_file(argv[0]);
  } else if (argc == 1) {
    run_prompt();
  } else {
    printf("Usage clox <filename>");
  }

}
