#include "stdio.h"

#include "stdlib.h"
#include "declarations.h"
#include "token.h"


Token token_create(TokenType type, char lexeme[], void *literal) {
  Token new_token = {.type = type, .literal = literal, .lexeme = *lexeme};
  return new_token;
}

void print_token(Token *token) {
  printf("Token\n");
  printf("\t%s", to_string(token->type));
  printf("\t%d", token->line);
}


char *to_string(TokenType type) {
  switch (type) {
    case LEFT_PAREN:      return "(";
    case RIGHT_PAREN:     return ")";
    case LEFT_BRACE:      return "{";
    case RIGHT_BRACE:     return "}";
    case COMMA:           return ",";
    case DOT:             return ".";
    case MINUS:           return "-";
    case PLUS:            return "+";
    case SEMICOLON:       return ";";
    case SLASH:           return "/";
    case STAR:            return "*";
    case BANG:            return "!";
    case BANG_EQUAL:      return "!=";
    case EQUAL:           return "=";
    case EQUAL_EQUAL:     return "==";
    case GREATER:         return ">";
    case GREATER_EQUAL:   return ">=";
    case LESS:            return "<";
    case LESS_EQUAL:      return ">=";
    case IDENTIFIER:      return "Identifier";
    case STRING:          return "String";
    case NUMBER:          return "Number"; 
    case AND:             return "and";
    case CLASS:           return "class";
    case ELSE:            return "else";
    case FALSE:           return "false";
    case FUN:             return "fun";
    case FOR:             return "for";
    case IF:              return "if";
    case NIL:             return "nil";
    case OR:              return "or";
    case PRINT:           return "print";
    case RETURN:          return "return";
    case SUPER:           return "super";
    case THIS:            return "this";
    case TRUE:            return "true";
    case VAR:             return "var";
    case WHILE:           return "while";
    case FILE_EOF:        return "eof";
  }
  

}

TokenArray token_array_create() {
  Token *tokens = calloc(INITIAL_TOKEN_ARRAY_SIZE, sizeof(Token));
  TokenArray token_array = { 
    .tokens = tokens,
    .size = 0,
    .used = 0,
  };
  return token_array;
}

void token_array_append(TokenArray *array, Token token) {
  if (array->used + 1 == array->size) {
    array->size = 2 * array->size;
    array->tokens = realloc(array->tokens, array->size);
  } 
  array->used++;
  array->tokens[array->used + 1] = token;

}
