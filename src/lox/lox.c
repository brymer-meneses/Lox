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
#include "lox/core.h"
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
  lox->interpreter = interpreter__init();
};

void lox__run(char* source) {
  assert(lox != NULL); /* Lox must be initialized */

  lox->source_code = source;
  interpreter__run(source);
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

    scanner__init(line);
    Token** tokens = scanner__scan();
    Parser* parser = parser__init(tokens);

    parser__parse();

    if (tokens[0]->type == SOURCE_END) break; 

    while ( parser->need_repl_resolution ) {
      parser->need_repl_resolution = false;

      char additional_line[MAX_INPUT_LIMIT];
      printf("... ");
      fgets(additional_line, sizeof(line) , stdin);
      strcat(line, additional_line);


      scanner__init(line);
      tokens = scanner__scan();
      parser = parser__init(tokens);
      parser__parse();
    }
       
    lox__run(line);
  }
}

void lox__free() {
  if (lox->source_code == NULL) return;
  // stmts__free(lox->interpreter->parser->num_stmts, lox->interpreter->parser->stmts);
  // tokens__free(lox->interpreter->scanner->tokens_array->curr_size, (Token**) lox->interpreter->scanner->tokens_array->elements);
  // environment__free(lox->interpreter->globals);

  free(lox->interpreter->scanner->tokens_array);
  free(lox->interpreter->scanner);
  free(lox->interpreter->parser);
  free(lox);
}
