#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "scanner.h"
#include "parser.h"

void raise_unexpected_character_error(const char chr);
void raise_unterminated_string_error(const char* lexeme);

void raise_expected_token_error(const char* lexeme, FileLoc fl);
#endif
