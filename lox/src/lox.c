#include "stdio.h"
#include "string.h"

#include "lox/astprinter.h"
#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/utils.h"
#include "lox/error.h"
#include "lox/state.h"
#include "lox/parser.h"
#include "lox/lox.h"

Lox lox;

void run(char source[]) {
  scanner_init(source);
  Token* tokens = scanner_scan();

  parser_init(tokens);
  Expr* expression = parser_parse();

  printf("%s\n", expr_to_string(expression));
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
    // printf("%s", line);
    
    run(line);
    if (strcmp(line, "\n") == 0)  break; 
  }

}
