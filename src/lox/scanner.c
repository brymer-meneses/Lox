#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "assert.h"

#include "tools/hashmap.h"
#include "tools/utils.h"
#include "tools/array.h"

#include "lox/fileloc.h"
#include "lox/error.h"
#include "lox/object.h"
#include "lox/token.h"
#include "lox/scanner.h"
#include "lox/core.h"
#include "lox/lox.h"


static void add_token(TokenType type, LoxObject* literal);
static char peek();
static bool match(char expected);
static char advance();
static bool isfinished();

static void scan_token();
static void scan_string();
static void scan_number();
static void scan_identifier();

static Scanner* scanner;
static TokenType keywords_match(char* text);

typedef struct Keyword {
  char* lexeme;
  TokenType type;
} Keyword;

static Keyword keywords[] = {
   {"and",    AND   },
   {"class",  CLASS },
   {"else",   ELSE  },
   {"false",  FALSE },
   {"for",    FOR   },
   {"fun",    FUN   },
   {"if",     IF    },
   {"nil",    NIL   },
   {"or",     OR    },
   {"print",  PRINT },
   {"return", RETURN},
   {"super",  SUPER },
   {"this",   THIS  },
   {"true",   TRUE  },
   {"var",    VAR   },
   {"while",  WHILE },
};

static const unsigned int keywords_length = sizeof(keywords) / sizeof(keywords[0]);


Scanner* scanner__init(char* source) {
  scanner = malloc(1 * sizeof(Scanner));
  scanner->current =0;
  scanner->start =0;
  scanner->line =1;
  scanner->source=source;
  scanner->last_line = 0;
  scanner->tokens_array = array__init(sizeof(Token*));
  return scanner;
}

Token** scanner__scan() {
  while (!isfinished()) {
    scanner->start = scanner->current;
    scan_token();
  }
  add_token(SOURCE_END, NULL);
  Token** tokens = (Token**) scanner->tokens_array->elements;;
  return tokens;
}

void scanner_free() {

}

static FileLoc* compute_relative_position() {
  FileLoc* fl = malloc(1 * sizeof(FileLoc));
  fl->line  = scanner->line;
  fl->start = scanner->start - scanner->last_line;
  fl->end   = scanner->current - 1 - scanner->last_line;
  return fl;
}

static void scan_token() {
  char c = advance();
  switch (c) {
    case '{':
      add_token(LEFT_BRACE, NULL);
      break;
    case '}': 
      add_token(RIGHT_BRACE, NULL);
      break;
    case '(':
      add_token(LEFT_PAREN, NULL);
      break;
    case ')':
      add_token(RIGHT_PAREN, NULL);
      break;
    case ',':
      add_token(COMMA, NULL);
      break;
    case '.':
      add_token(DOT, NULL);
      break;
    case '*':
      add_token(STAR, NULL);
      break;
    case '+':
      add_token(PLUS, NULL);
      break;
    case '-':
      add_token(MINUS, NULL);
      break;
    case ';':
      add_token(SEMICOLON, NULL);
      break;
    case '^':
      add_token(POW, NULL);
      break;
    case '>':
      if (match('=')) 
        add_token(GREATER_EQUAL, NULL);
      else
        add_token(GREATER, NULL);
      break;
    case '<':
      if (match('=')) 
        add_token(LESS_EQUAL, NULL);
      else
        add_token(LESS, NULL);
      break;

    case '=':
      if (match('=')) 
        add_token(EQUAL_EQUAL, NULL);
      else 
        add_token(EQUAL, NULL);
      break;

    case '!':
      if (match('='))
        add_token(BANG_EQUAL, NULL);
      else 
        add_token(BANG, NULL);
      break;
    case '/':
      if (match('/')) {
        while(peek() != '\n')
          advance();
      } else {
        add_token(SLASH, NULL);
      }
       break;
    
    case '"':
      scan_string();
      break;
    case ' ':
    case '\t':
    case '\r':
      // ignore whitespaces
      break;

    case '\n': 
      scanner->line++;
      scanner->last_line = scanner->current;
      break;
    default:
      if (isdigit(c)) {
        scan_number();
      } else if (isalpha(c) || c == '_') {
        scan_identifier();
      } else {
        report(fileloc__init(scanner->line, scanner->current - 1, scanner->current -1), "Got unexpected character: %c", c);
      }
      break;
  }

}

static void add_token(TokenType type, LoxObject* literal) {
  char* lexeme = substring(scanner->source, scanner->start, scanner->current-1); // current points to the next "char" so we subtract by 1
  FileLoc* fl  =  compute_relative_position();
  Token* token = token__init(type, lexeme, literal, fl);

  array__append(scanner->tokens_array, token);
}

static char advance() {
  if (isfinished()) return '\0';

  scanner->current++;
  return scanner->source[scanner->current - 1];
}

static void scan_number() {

  while (isdigit(peek())) 
    advance();

  
  if (peek() == '.') {
    advance();
    while (isdigit(peek())) 
      advance();
  } 
  char* num_lexeme = substring(scanner->source, scanner->start, scanner->current-1);
  FileLoc* fl = compute_relative_position();
  LoxObject* literal = loxobject__init(LOX_NUMBER, num_lexeme, fl);

  add_token(NUMBER, literal);

}

static void scan_string() {
  while (peek() != '"' && !isfinished()) {
    if (peek() == '\n') scanner->line++;
    advance();
  }

  // handle when the string doesn't terminate
  if (isfinished()) {
    report(compute_relative_position(), "Expected a closing \" at this position.");
    return;
  }
  // consume the last '"' character
  advance();

  char* text = substring(scanner->source, scanner->start+1, scanner->current-2);
  FileLoc* fl = compute_relative_position();

  LoxObject* literal = loxobject__init(LOX_STRING, text, fl);
  add_token(STRING, literal); 
}

static void scan_identifier() {
  while (isalnum(peek()) || peek() == '_')
    advance();

  char* identifier = substring(scanner->source, scanner->start, scanner->current-1);
  TokenType type = keywords_match(identifier);

  add_token(type, NULL);
}


static bool isfinished() {
  return scanner->current >= strlen(scanner->source);
}

static char peek() {
  if (isfinished()) return '\0';
  
  return scanner->source[scanner->current];
}

static char peek_next() {
  if (isfinished()) return '\0';
  
  return scanner->source[scanner->current+1];
}

static bool match(char expected) {
  if (peek() == expected) {
    scanner->current++;
    return true;
  }
  return false;
}


static TokenType keywords_match(char* text) {
  for (unsigned int i=0; i <keywords_length; i++) {
    if (strcmp(text, keywords[i].lexeme) == 0) {
      return keywords[i].type;
    }
  }
  return IDENTIFIER;
}
