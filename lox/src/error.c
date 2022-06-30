#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

#include "lox/state.h"
#include "lox/error.h"
#include "lox/scanner.h"
#include "lox/parser.h"

#define COLOR(COLOR, STRING) COLOR STRING ANSI_COLOR_RESET
#define SET_COLOR(COLOR) printf(COLOR)

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


static void point_error_root(const size_t line_number, const char* source, const size_t begin, const size_t end) {

  char source_context[128];
  char formatted_source[strlen(source) + strlen(source_context)];

  sprintf(source_context, "    %lu | ", line_number);
  sprintf(formatted_source, "%s%s", source_context, source);

  const size_t offsetted_begin = begin + strlen(source_context);
  const size_t offsetted_end = end + strlen(source_context);

  char* pointer_str = strdup(formatted_source);
  for (size_t i=0; i<strlen(pointer_str); i++) {

    if (i>=offsetted_begin && i<= offsetted_end) {
      pointer_str[i] = '^';
    } else {
      pointer_str[i] = ' ';
    }
  }

  SET_COLOR(ANSI_COLOR_RESET);
  printf("%s\n", formatted_source);
  SET_COLOR(ANSI_COLOR_CYAN);
  printf("%s\n", pointer_str);
}


void raise_unterminated_string_error(Scanner* s, const char* lexeme) {
  const int current_pos_rel_line = s->start - s->last_line;

  point_error_root(s->line, scanner_get_current_line(s), current_pos_rel_line, current_pos_rel_line + strlen(lexeme));

  printf( COLOR(ANSI_COLOR_RED, "  ERROR: ") "Unterminated string, did you forget to place \"?\n");
}

void raise_unexpected_character_error(Scanner* s, const char chr) {
  const int current_pos_rel_line = s->current - 1 - s->last_line;
  point_error_root(s->line, scanner_get_current_line(s), current_pos_rel_line, current_pos_rel_line);

  printf( COLOR(ANSI_COLOR_RED, "  ERROR: ") "Unexpected character: %c\n", chr);
}
