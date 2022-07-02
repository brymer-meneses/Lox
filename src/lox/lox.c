#include "stdio.h"
#include "string.h"

#include "tools/astprinter.h"
#include "tools/utils.h"
#include "tools/error.h"

#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/parser.h"
#include "lox/lox.h"
#include "lox/interpreter.h"

Lox lox;

FileLoc FILE_LOC_NULL = {.start=0, .end=0, .line=0};
LoxObject LOX_OBJECT_NULL = {.type = NIL};
Token TOKEN_NULL = {.type = NIL, 0, "NIL", .literal=(LoxObject){.type = NIL}, .fileloc={0, 0, 0}};

void lox_init() {
  lox.had_runtime_error = false;
  lox.had_error = false;
}

void run(char source[]) {
  lox_init();
  
  scanner_init(source);
  Token* tokens = scanner_scan();
  
  parser_init(tokens);
  Expr* expression = parser_parse();

  interpret(expression);
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

  while(true) {
    char line[MAX_INPUT_LIMIT];

    printf("> ");
    fgets(line, sizeof(line) , stdin);
    
    run(line);
    if (strcmp(line, "\n") == 0)  break; 
  }

}
