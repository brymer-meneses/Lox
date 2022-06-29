#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

#include "lox/state.h"
#include "lox/error.h"
#include "lox/scanner.h"
#include "lox/parser.h"

ErrorContext errorcontext_scanner_init(const Scanner* s, const char* lexeme) {

  // pointer to lexeme relative to the line
  const int start =  (s->current - 1) - s->last_line;
  return (ErrorContext) {
    .column = s->current,
    .source_line = scanner_get_current_line(s),
    .line = s->line,

    // pointer to lexeme relative to the line
    .lexeme_start = start,
    .lexeme_end = start + strlen(lexeme),
    .lexeme = lexeme,
  };
}

void highlight_range(const char* line, const size_t start, const size_t end) {
  printf("\n%s\n", line);

  char highlight[strlen(line)];

  for (int i=0; i<strlen(line); i++) {
    if (i<start) {
      highlight[i] = ' ';
      continue;
    }

    if (i >= start && i < end)
      highlight[i] = '^';
    else
      highlight[i] = ' ';

  }
  highlight[strlen(line)] = '\0';
  printf("%s\n", highlight);
}

/**
  * Prints the character ^ underneath the first instance of the substring "lexeme" of the variable "line."
  * This is used to direct the attention of the user to the lexeme that has an error.
  */
void highlight_lexeme(const char* line, const char* lexeme) {
  char* result = strstr(line, lexeme);

  // calculate the index of the first instance of "lexeme" in "line"
  const int start = result - line; 
  const int end = strlen(lexeme);

  highlight_range(line, start, end);
}

void report(LoxErrorType type, ErrorContext context, char* message) {

  char source[strlen(message) + 128];
  char offset[16];
  sprintf(offset, "%lu | ", context.line);
  sprintf(source, "%s%s\n", offset, context.source_line);

  const char offset_length = strlen(offset);

  highlight_range(source, offset_length + context.lexeme_start, offset_length + context.lexeme_end);

  switch (type) {
    case LOX_RUNTIME_ERROR:
      sprintf(source, "Runtime Error: %s\n", message);
      break;
    case LOX_SYNTAX_ERROR:
      sprintf(source, "Syntax Error: %s\n", message);
      break;
    case LOX_INTERPRETER_ERROR:
      sprintf(source, "Interpreter Error: %s\n", message);
      break;
  }

  fprintf(stderr, "%s", source);
  return;
}
