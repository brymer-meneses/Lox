#include "lox/object.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "assert.h"
#include "stdlib.h"

#include "lox/core.h"
#include "lox/fileloc.h"
#include "lox/token.h"

#include "tools/utils.h"

Token* token__init(TokenType type, char* lexeme, LoxObject* literal, FileLoc* fileloc) {
  Token* tok = malloc(1 * sizeof(Token));
  tok->type = type;
  tok->literal = literal;
  tok->fileloc = fileloc;

  tok->lexeme = malloc(sizeof(char) * (strlen(lexeme) + 1));
  strcpy(tok->lexeme, lexeme);
  return tok;
}


void token__print(Token* token) {
  // do not print SOURCE_END token
  if (token->type == SOURCE_END) return;

  printf("Token: [ ");
  printf("type: %s, ", tokentype__to_string(token->type));
  printf("literal: %s, ", loxobject__to_string(token->literal));
  printf("line: %lu ", token->fileloc->line);
  printf("]\n");
}

void token__free(Token* token) {
  if (token == NULL) return;
  loxobject__free(token->literal);
  free(token->fileloc);
  free(token->lexeme);
  free(token);
}

void tokens__free(size_t num_tokens, Token** tokens) {
  for (size_t i=0; i<num_tokens; i++) {
    token__free(tokens[i]);
  }
  free(tokens);
}


char* tokentype__to_string(TokenType type) {
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
    case SOURCE_END:      return "EOF";
    case POW:             return "POW"; 
  }
}
