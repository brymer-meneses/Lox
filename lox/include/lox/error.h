#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "scanner.h"

typedef enum {
  LOX_RUNTIME_ERROR,
  LOX_SYNTAX_ERROR,
  LOX_INTERPRETER_ERROR,
} LoxErrorType;

typedef struct {
  const size_t column;
  const size_t line;
  const char* source_line;
  const char* lexeme;
  const unsigned int lexeme_start;
  const unsigned int lexeme_end;
} ErrorContext;

void report(LoxErrorType type, ErrorContext context, char* message);
void highlight_lexeme(const char* line, const char* lexeme);
ErrorContext errorcontext_scanner_init(const Scanner* s, const char* lexeme);

#endif
