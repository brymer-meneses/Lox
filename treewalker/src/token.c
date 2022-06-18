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

