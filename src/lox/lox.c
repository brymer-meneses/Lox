#include "lox/stmt.h"
#include "stdio.h"
#include "string.h"

#include "tools/astprinter.h"
#include "tools/hashmap.h"
#include "tools/utils.h"
#include "tools/error.h"
#include "tools/debug.h"

#include "lox/environment.h"
#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/parser.h"
#include "lox/lox.h"
#include "lox/interpreter.h"

#include "assert.h"

static Lox* lox;

Lox* lox__init() {
  lox = malloc(1 * sizeof(Lox));
  lox->status.had_error = false;
  lox->status.had_runtime_error = false;

  lox->context.environment = environment_init();
  return lox;
};

void lox__run(char* source) {
  assert(lox != NULL); /* Lox must be initialized */

  lox->context.source_code = source;
  lox->context.scanner = scanner_init(source);

  Token** tokens = scanner_scan();
  lox->context.parser =  parser_init(tokens);
  Stmt** stmts = parser_parse(tokens);

  interpret(
      stmts,
      lox->context.environment,
      lox->context.parser->num_stmts
  );
};

void lox__run_file(const char* filename) {

  char* contents = read_file(filename);
  if (contents == NULL) {
    fprintf(stderr, "ERROR: Cannot read file: %s.\n", filename);
    return;
  }

  lox__run(contents);
  free(contents);
};

Lox* lox__get() {
  return lox;
};

void lox__run_prompt() {
  lox->status.is_on_repl = true;

  while(true) {
    lox->status.had_error = false;
    char line[MAX_INPUT_LIMIT];

    printf("> ");
    fgets(line, sizeof(line) , stdin);
   
    lox__run(line);
    if (strcmp(line, "\n") == 0)  break;
  }
}

void lox__free() {
  stmts_free(lox->context.parser->num_stmts, lox->context.parser->stmts);
  tokens_free(lox->context.scanner->tokens_array->curr_size, (Token**) lox->context.scanner->tokens_array->elements);
  hashmap_free(lox->context.environment->values);

  free(lox->context.scanner->tokens_array);
  free(lox->context.scanner);
  free(lox->context.parser);
  free(lox->context.environment);
  free(lox);
}
