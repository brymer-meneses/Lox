#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "lox/scanner.h"
#include "lox/parser.h"

void raise_unexpected_character_error(const char chr);
void raise_unterminated_string_error();

void raise_expected_token_error(const char* lexeme, FileLoc fl);
void raise_different_operands_error(const char* message, FileLoc fl);

void raise_invalid_binary_operation_error(Token op, TokenType t1, TokenType t2);
#endif
