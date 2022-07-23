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


static void add_token(Scanner* s, TokenType type, LoxObject* literal);
static char peek(Scanner* s);
static bool match(Scanner* s, char expected);
static char advance(Scanner* s);
static bool isfinished(Scanner* s);

static void scan_token(Scanner* s);
static void scan_string(Scanner* s);
static void scan_number(Scanner* s);
static void scan_identifier(Scanner* s);

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
  Scanner* scanner = malloc(1 * sizeof(Scanner));
  scanner->current =0;
  scanner->start =0;
  scanner->line =1;
  scanner->source=source;
  scanner->last_line = 0;
  scanner->tokens_array = array__init(sizeof(Token*));
  return scanner;
}

Token** scanner__scan(Scanner* s) {
  while (!isfinished(s)) {
    s->start = s->current;
    scan_token(s);
  }
  add_token(s, SOURCE_END, NULL);
  Token** tokens = (Token**) s->tokens_array->elements;;
  return tokens;
}

void scanner_free(Scanner* s) {

}

static FileLoc* compute_relative_position(Scanner* s) {
  FileLoc* fl = malloc(1 * sizeof(FileLoc));
  fl->line  = s->line;
  fl->start = s->start - s->last_line;
  fl->end   = s->current - 1 - s->last_line;
  return fl;
}

static void scan_token(Scanner* s) {
  char c = advance(s);
  switch (c) {
    case '{':
      add_token(s, LEFT_BRACE, NULL);
      break;
    case '}': 
      add_token(s, RIGHT_BRACE, NULL);
      break;
    case '(':
      add_token(s, LEFT_PAREN, NULL);
      break;
    case ')':
      add_token(s, RIGHT_PAREN, NULL);
      break;
    case ',':
      add_token(s, COMMA, NULL);
      break;
    case '.':
      add_token(s, DOT, NULL);
      break;
    case '*':
      add_token(s, STAR, NULL);
      break;
    case '+':
      add_token(s, PLUS, NULL);
      break;
    case '-':
      add_token(s, MINUS, NULL);
      break;
    case ';':
      add_token(s, SEMICOLON, NULL);
      break;
    case '^':
      add_token(s, POW, NULL);
      break;
    case '>':
      if (match(s, '=')) 
        add_token(s, GREATER_EQUAL, NULL);
      else
        add_token(s, GREATER, NULL);
      break;
    case '<':
      if (match(s, '=')) 
        add_token(s, LESS_EQUAL, NULL);
      else
        add_token(s, LESS, NULL);
      break;

    case '=':
      if (match(s, '=')) 
        add_token(s, EQUAL_EQUAL, NULL);
      else 
        add_token(s, EQUAL, NULL);
      break;

    case '!':
      if (match(s, '='))
        add_token(s, BANG_EQUAL, NULL);
      else 
        add_token(s, BANG, NULL);
      break;
    case '/':
      if (match(s, '/')) {
        while(peek(s) != '\n')
          advance(s);
      } else {
        add_token(s, SLASH, NULL);
      }
       break;
    
    case '"':
      scan_string(s);
      break;
    case ' ':
    case '\t':
    case '\r':
      // ignore whitespaces
      break;

    case '\n': 
      s->line++;
      s->last_line = s->current;
      break;
    default:
      if (isdigit(c)) {
        scan_number(s);
      } else if (isalpha(c) || c == '_') {
        scan_identifier(s);
      } else {
        report(fileloc__init(s->line, s->current - 1, s->current -1), "Got unexpected character: %c", c);
      }
      break;
  }

}

static void add_token(Scanner* s, TokenType type, LoxObject* literal) {
  char* lexeme = substring(s->source, s->start, s->current-1); // current points to the next "char" so we subtract by 1
  FileLoc* fl  =  compute_relative_position(s);
  Token* token = token__init(type, lexeme, literal, fl);

  array__append(s->tokens_array, token);
}

static char advance(Scanner* s) {
  if (isfinished(s)) return '\0';

  s->current++;
  return s->source[s->current - 1];
}

static void scan_number(Scanner* s) {

  while (isdigit(peek(s))) 
    advance(s);

  
  if (peek(s) == '.') {
    advance(s);
    while (isdigit(peek(s))) 
      advance(s);
  } 
  char* num_lexeme = substring(s->source, s->start, s->current-1);
  FileLoc* fl = compute_relative_position(s);
  LoxObject* literal = loxobject__init(LOX_NUMBER, num_lexeme, fl);

  add_token(s, NUMBER, literal);

}

static void scan_string(Scanner* s) {
  while (peek(s) != '"' && !isfinished(s)) {
    if (peek(s) == '\n') s->line++;
    advance(s);
  }

  // handle when the string doesn't terminate
  if (isfinished(s)) {
    report(compute_relative_position(s), "Expected a closing \" at this position.");
    return;
  }
  // consume the last '"' character
  advance(s);

  char* text = substring(s->source, s->start+1, s->current-2);
  FileLoc* fl = compute_relative_position(s);

  LoxObject* literal = loxobject__init(LOX_STRING, text, fl);
  add_token(s, STRING, literal); 
}

static void scan_identifier(Scanner* s) {
  while (isalnum(peek(s)) || peek(s) == '_')
    advance(s);

  char* identifier = substring(s->source, s->start, s->current-1);
  TokenType type = keywords_match(identifier);

  add_token(s, type, NULL);
}


static bool isfinished(Scanner* s) {
  return s->current >= strlen(s->source);
}

static char peek(Scanner* s) {
  if (isfinished(s)) return '\0';
  
  return s->source[s->current];
}

static char peek_next(Scanner* s) {
  if (isfinished(s)) return '\0';
  
  return s->source[s->current+1];
}

static bool match(Scanner* s, char expected) {
  if (peek(s) == expected) {
    s->current++;
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
