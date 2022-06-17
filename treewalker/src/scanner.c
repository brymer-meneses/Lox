#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "token.h"
#include "scanner.h"

bool _is_at_end(Scanner *scanner);

Scanner scanner_create(char* source) {
  TokenArray tokens = token_array_create();

  Scanner scanner = {
    .current =0, 
    .start =0, 
    .line =1, 
    .source=source,
    .tokens=tokens,
  };

  return scanner;
}

void scanner_scan(Scanner* scanner) {
  
}

void _classify_token() {

}

void _add_token(Scanner *scanner) {

}


char _advance(Scanner *scanner) {
  return scanner->source[scanner->current + 1];
}


bool _is_at_end(Scanner *scanner) {
  return scanner->current >= strlen(scanner->source);
}

char _peek(Scanner *scanner) {
  return scanner->source[scanner->current];
}
