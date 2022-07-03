#ifndef LOX_OBJECT_H

#define LOX_OBJECT_H

#include "stdbool.h"

typedef enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  SOURCE_END
} TokenType;

typedef struct {
  union {
    double number;
    char* string;
    bool boolean;
  } data;
  TokenType type;
} LoxObject;


LoxObject encode_bool(bool value);
LoxObject encode_string(const char* value);
LoxObject encode_double(double value);

char* loxobject_to_string(LoxObject obj);

#endif // !LOX_OBJECT_H


