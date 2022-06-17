#include "stdio.h"

#include "stdlib.h"
#include "declarations.h"
#include "token.h"


Token token_create(TokenType type, char* lexeme, void *literal) {
  Token token = {.type = type, .literal = literal, .lexeme = lexeme};
  return token;
}

void token_print(Token *token) {
  printf("Token\n");
  printf("\t%s", token_to_string(token->type));
  printf("\t%zu", token->line);
}


char *token_to_string(TokenType type) {
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

