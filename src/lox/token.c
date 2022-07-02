#include "lox/object.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "assert.h"
#include "stdlib.h"

#include "lox/filelocation.h"
#include "lox/declarations.h"
#include "lox/token.h"
#include "tools/utils.h"

Token token_init(TokenType type, char* lexeme, LoxObject literal, FileLoc fileloc) {
  return (Token) {.type = type, .lexeme = lexeme, .literal= literal, .fileloc = fileloc};
}


void token_print(Token token) {
  // do not print SOURCE_END token
  if (token.type == SOURCE_END) return;

  printf("Token: [ ");
  printf("type: %s, ", tokentype_to_string(token.type));
  printf("literal: %s, ", token_to_string(token));
  printf("line: %lu ", token.line);
  printf("]\n");
}


char *tokentype_to_string(TokenType type) {
  switch (type) {
    case LEFT_PAREN:      return "LEFT_PAREN";
    case RIGHT_PAREN:     return "RIGHT_PAREN";
    case LEFT_BRACE:      return "LEFT_BRACE"; 
    case RIGHT_BRACE:     return "RIGHT_BRACE";
    case COMMA:           return "COMMA"; 
    case DOT:             return "DOT";          
    case MINUS:           return "MINUS";        
    case PLUS:            return "PLUS";        
    case SEMICOLON:       return "SEMICOLON";
    case SLASH:           return "SLASH";
    case STAR:            return "STAR";         
    case BANG:            return "BANG";         
    case BANG_EQUAL:      return "BANG_EQUAL";   
    case EQUAL:           return "EQUAL";        
    case EQUAL_EQUAL:     return "EQUAL_EQUAL"; 
    case GREATER:         return "GREATER";      
    case GREATER_EQUAL:   return "GREATER_EQUAL";
    case LESS:            return "LESS";         
    case LESS_EQUAL:      return "LESS_EQUAL";   
    case IDENTIFIER:      return "IDENTIFIER";
    case STRING:          return "STRING";
    case NUMBER:          return "NUMBER"; 
    case AND:             return "AND";
    case CLASS:           return "CLASS";
    case ELSE:            return "ELSE";
    case FALSE:           return "FALSE";
    case FUN:             return "FUN";
    case FOR:             return "FOR";
    case IF:              return "IF";
    case NIL:             return "NIL";
    case OR:              return "OR";
    case PRINT:           return "PRINT";
    case RETURN:          return "RETURN";
    case SUPER:           return "SUPER";
    case THIS:            return "THIS";
    case TRUE:            return "TRUE";
    case VAR:             return "VAR";
    case WHILE:           return "WHILE";
    case SOURCE_END:        return "EOF";
  }
}

double token_parse_double(Token token) {
  return strtod(token.lexeme, NULL);
}

bool token_parse_bool(Token token) {
  bool istrue = strcmp(token.lexeme, "true") == 0;
  bool isfalse = strcmp(token.lexeme, "false") == 0;

  assert(istrue || isfalse);

  if (istrue) {
    return true; 
  }; 

  return false; 
}

char* token_parse_string(Token token) {
  return substring(token.lexeme, 1, strlen(token.lexeme) - 2); // trim the quotation marks
}


char* token_to_string(Token token) {
  return loxobject_to_string(token.literal);
}
