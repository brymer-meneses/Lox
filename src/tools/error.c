#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

#include "tools/termcolor.h"
#include "tools/utils.h"
#include "tools/error.h"

#include "lox/filelocation.h"
#include "lox/token.h"
#include "lox/scanner.h"
#include "lox/parser.h"
#include "lox/lox.h"

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


void raise_unterminated_string_error() {

  char* line = get_current_line();

  point_error_root(line, compute_relative_position());

  printf( COLOR(ANSI_CODE_RED, "  ERROR: ") "Unterminated string, did you forget to place \"?\n");
  lox.had_error = true;
}

void raise_unexpected_character_error(const char chr) {

  char* line = get_current_line();

  point_error_root(line, compute_relative_position());

  printf( COLOR(ANSI_CODE_RED, "  ERROR: ") "Unexpected character: %c\n", chr);
  lox.had_error = true;
  free(line);
}

void raise_expected_token_error(const char* lexeme, FileLoc fl) {

  char* line = get_current_line();

  point_error_root(line, fl);
  printf( COLOR(ANSI_CODE_RED, "  ERROR: ") "Expected matching %s of this character.", lexeme);

  lox.had_error = true;
  free(line);
}
