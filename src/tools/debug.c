#include "stdio.h"

#include "lox/token.h"
#include "tools/astprinter.h"
#include "tools/debug.h"

void log_tokens(Token* tokens) {
  for (int i=0; tokens[i].type != SOURCE_END; i++) {
    token_print(tokens[i]);
  }
}

void log_ast(Expr* ast) {
  printf("%s", expr_to_string(ast));
}
