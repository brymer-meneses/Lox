#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"

#include "lox/astprinter.h"
#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/utils.h"
#include "lox/error.h"
#include "lox/state.h"


void run(char source[]) {
  Scanner scanner = scanner_init(source);
  Token* tokens = scanner_scan(&scanner);

  for (int i=0; i<scanner.parsed; i++) {
    token_print(tokens[i]);
  }

}

void run_file(const char* filename) {

  char* contents = read_file(filename);
  if (contents == NULL) {
    fprintf(stderr, "ERROR: Cannot read file: %s.\n", filename);
    return;
  }
  run(contents);
  free(contents);
}

void run_prompt() {

  while(true && !LOX_HAD_ERROR) {
    char line[MAX_INPUT_LIMIT];

    printf("> ");
    fgets(line, sizeof(line) , stdin);
    
    run(line);
    if (strcmp(line, "\n") == 0)  break; 
  }

}

int main(int argc, char *argv[]) {


  if (argc == 2) {
    run_file(argv[0]);
  } else if (argc == 1) {
    run_prompt();
  } else {
    printf("Usage clox <filename>");
  }
}
