#include "lox/filelocation.h"
#include "lox/utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

#include "termcolor.h"

#include "lox/state.h"
#include "lox/error.h"
#include "lox/scanner.h"
#include "lox/parser.h"

#define COLOR(COLOR, STRING) COLOR STRING ANSI_CODE_RESET

static char* init_empty_string(const int size) {
  char* str = malloc(size * sizeof(char));

  for (int i=0; i<size; i++) {
    str[i] = ' ';
  }
  str[size] = '\0';
  return str;
}


static void point_error_root(const char* source, FileLoc fl) {

  char source_context[64];
  sprintf(source_context, "    %lu| ", fl.line);

  const unsigned int source_context_size = strlen(source_context);
  const unsigned int source_size = strlen(source);

  char* pointer_str = init_empty_string(source_size);
  char* pointer_str_offset = init_empty_string(source_context_size);

  for (size_t i=0; i<strlen(source); i++) {
    if (i>=fl.start && i<= fl.end) {
      pointer_str[i] = '^';
    }
  }

  printf( COLOR(ANSI_CODE_CYAN, "%s"), source_context);
  printf("%s\n", source);

  printf("%s", pointer_str_offset);
  printf( COLOR(ANSI_CODE_CYAN, "%s\n"), pointer_str);

  free(pointer_str);
  free(pointer_str_offset);
}


void raise_unterminated_string_error(Scanner* s, const char* lexeme) {

  point_error_root(scanner_get_current_line(s), compute_relative_position(s));

  printf( COLOR(ANSI_CODE_RED, "  ERROR: ") "Unterminated string, did you forget to place \"?\n");
  LOX_HAD_ERROR = true;
}

void raise_unexpected_character_error(Scanner* s, const char chr) {

  point_error_root(scanner_get_current_line(s), compute_relative_position(s));

  printf( COLOR(ANSI_CODE_RED, "  ERROR: ") "Unexpected character: %c\n", chr);
  LOX_HAD_ERROR = true;
}

void raise_expected_token_error(Parser* p, Token type) {

  FileLoc fl = type.fileloc;
  char* current_line = str_split(p->source, "\n")[fl.line - 1];

  point_error_root(current_line, fl);
  printf( COLOR(ANSI_CODE_RED, "  ERROR: ") "Expected natching token here.");

  LOX_HAD_ERROR = true;
}
