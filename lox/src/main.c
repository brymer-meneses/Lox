#include "lox/astprinter.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "lox/expr.h"


void run(char source[]) {
  Scanner scanner = scanner_init(source);
  Token* tokens = scanner_scan(&scanner);

  for (int i=0; i<scanner.parsed; i++) {
    token_print(tokens[i]);
  }

}

void run_file(char filename[]) {

}

void run_prompt() {

  while(true) {
    char line[MAX_INPUT_LIMIT];

    printf("> ");
    fgets(line, sizeof(line) , stdin);
    
    // consume the '\n' character at the end of the string
    // line[strcspn(line, "\n")]  = 0; 

    run(line);
    if (strcmp(line, "\n") == 0)  break; 
  }

}

int main(int argc, char *argv[]) {

  // if (argc == 2) {
  //   run_file(argv[0]);
  // } else if (argc == 1) {
  //   run_prompt();
  // } else {
  //   printf("Usage clox <filename>");
  // }
  //

  Expr* expression = binary(
      unary(
        token_init(MINUS, "-", "Null", 1),
        literal("123")),
      token_init(STAR, "*", "Null", 1),
      grouping(literal("45.67")));
  printf("%s\n", expr_to_string(expression));
}
