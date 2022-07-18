#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "assert.h"

#include "tools/termcolor.h"
#include "tools/utils.h"
#include "tools/fileloc.h"

#include "lox/token.h"
#include "lox/error.h"
#include "lox/scanner.h"
#include "lox/parser.h"
#include "lox/lox.h"

#define COLOR(COLOR, STRING) COLOR STRING ANSI_CODE_RESET

static char* get_source_line(const size_t line_num) {
  char** arr = str_split(lox->context.source_code, "\n");

  return arr[line_num-1];
}

static char* init_empty_string(const int size) {
  char* str = malloc(size * sizeof(char) + 1);

  for (int i=0; i<size; i++) {
    str[i] = ' ';
  }
  str[size] = '\0';
  return str;
}


// TODO: print for multiple lines
static void point_error_root(char* source, FileLoc* fl) {
  assert(source != NULL);
  assert(fl != NULL);

  char source_context[64];
  if (lox->status.is_on_repl)
    sprintf(source_context, "    > | ");
  else
    sprintf(source_context, "    %lu| ", fl->line);

  const size_t source_context_size = strlen(source_context);
  const size_t source_size = strlen(source);

  char* pointer_str = init_empty_string(source_size);
  char* pointer_str_offset = init_empty_string(source_context_size);
  
  for (size_t i=0; i<strlen(source); i++) {
    if (i>=fl->start && i<= fl->end) {
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

void report(FileLoc* fl, const char* message, ...) {

  if (lox == NULL) return;

  if (fl != NULL)  {
    char* line = get_source_line(fl->line);
    point_error_root(line, fl);
  }

  char* formatted_message = str_concat(COLOR(ANSI_CODE_RED, "  ERROR: "), message);

  // Put newline
  formatted_message = str_concat(formatted_message, "\n");

  // Print output
  va_list args;
  va_start(args, message);

  vfprintf(stdout, formatted_message, args);

  va_end(args);

  lox->status.had_error = true;
}

