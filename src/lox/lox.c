#include "lox/environment.h"
#include "stdio.h"
#include "string.h"

#include "tools/astprinter.h"
#include "tools/hashmap.h"
#include "tools/utils.h"
#include "tools/error.h"
#include "tools/debug.h"

#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/parser.h"
#include "lox/lox.h"
#include "lox/interpreter.h"

Lox lox;

void lox_init() {
  lox.had_runtime_error = false;
  lox.had_error = false;
}

void run(char source[]) {
  
  scanner_init(source);
  Token** tokens = scanner_scan();

  // log_tokens(tokens);

  parser_init(tokens);
  Stmt** statements = parser_parse();

  interpret(statements);

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
  lox.is_on_repl = true;
  lox_init();
  environment_init();

  while(true) {
    char line[MAX_INPUT_LIMIT];

    printf("> ");
    fgets(line, sizeof(line) , stdin);
    
    run(line);
    if (strcmp(line, "\n") == 0)  break; 
  }

  hashmap_free(lox.environment.values);
}
