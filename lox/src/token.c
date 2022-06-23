#include "stdio.h"
#include "strings.h"
#include "stdlib.h"

#include "lox/declarations.h"
#include "lox/literal.h"
#include "lox/token.h"


Token token_init(TokenType type, char* lexeme, Literal literal, unsigned int line) {
  Token token = {.type = type, .literal = literal, .lexeme = lexeme, .line = line};
  return token;
}

void token_print(Token token) {
  // do not print FILE_EOF token
  if (token.type == FILE_EOF) return;

  printf("Token: [ ");
  printf("type: %s, ", token_to_string(token.type));
  switch (token.type) {
     case STRING:
        printf("literal: %s ", token.literal);
        break;
     case NUMBER:
        printf("literal: %f ", literal_parse_double(token.literal));
        break;
     default:
        break;
  }
  printf("line: %lu ", token.line);
  printf("]\n");
}


char *token_to_string(TokenType type) {
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
    case FILE_EOF:        return "EOF";
  }
  

}

