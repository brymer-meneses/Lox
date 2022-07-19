#include "lox/stmt.h"
#include "stdio.h"
#include "string.h"

#include "tools/astprinter.h"
#include "tools/hashmap.h"
#include "tools/utils.h"
#include "tools/debug.h"

#include "lox/error.h"
#include "lox/environment.h"
#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/parser.h"
#include "lox/lox.h"
#include "lox/interpreter.h"

#include "assert.h"

Lox* lox = NULL;

void lox__init() {
  lox = malloc(1 * sizeof(Lox));
  lox->status.had_error = false;
  lox->status.had_runtime_error = false;

  lox->context.environment = environment_init(NULL);
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


void lox__run_prompt() {
  lox->status.is_on_repl = true;

  putchar('\n');
  puts("Lox");
  puts("A treewalker interpreter of the Lox language in C!");
  puts("© Brymer Meneses");
  putchar('\n');


  while(true) {
    lox->status.had_error = false;
    char line[MAX_INPUT_LIMIT];

    printf(">>> ");
    fgets(line, sizeof(line) , stdin);

    scanner_init(line);
    Token** tokens = scanner_scan();
    Parser* parser = parser_init(tokens);

    parser_parse();

    if (tokens[0]->type == SOURCE_END) break; 

    while ( parser->need_repl_resolution ) {
      parser->need_repl_resolution = false;

      char additional_line[MAX_INPUT_LIMIT];
      printf("... ");
      fgets(additional_line, sizeof(line) , stdin);
      strcat(line, additional_line);


      scanner_init(line);
      tokens = scanner_scan();
      parser = parser_init(tokens);
      parser_parse();
    }
       
    lox__run(line);
  }
}

void lox__free() {
  if (lox->context.source_code == NULL) return;
  stmts_free(lox->context.parser->num_stmts, lox->context.parser->stmts);
  tokens_free(lox->context.scanner->tokens_array->curr_size, (Token**) lox->context.scanner->tokens_array->elements);
  environment_free(lox->context.environment);

  free(lox->context.scanner->tokens_array);
  free(lox->context.scanner);
  free(lox->context.parser);
  free(lox->context.environment);
  free(lox);
}
