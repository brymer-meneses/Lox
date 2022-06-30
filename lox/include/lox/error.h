#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "scanner.h"

void raise_unexpected_character_error(Scanner* s, const char chr);
void raise_unterminated_string_error(Scanner* s, const char* lexeme);

#endif
