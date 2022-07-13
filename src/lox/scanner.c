#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "assert.h"

#include "tools/error.h"
#include "tools/hashmap.h"
#include "tools/utils.h"
#include "tools/fileloc.h"
#include "tools/hashmap.h"

#include "lox/object.h"
#include "lox/token.h"
#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/lox.h"

static TokenType keywords_match(char* text);
static void keywords_set(char* text, TokenType type);

static void add_token(TokenType type, LoxObject* literal);
static char peek();
static bool match(char expected);
static char advance();
static bool isfinished();

static void scan_token();
static void scan_identifier();
static void scan_string();
static void scan_number();

static Hashmap* keywords;


void scanner_init(const char* source) {
  lox.scanner = (Scanner) {
    .current =0, 
    .start =0, 
    .line =1, 
    .source=source,
    .capacity = INITIAL_TOKEN_ARRAY_SIZE,
    .parsed = 0,
    .last_line = 0,
    .tokens = calloc(INITIAL_TOKEN_ARRAY_SIZE, sizeof(Token*)),
  };

  keywords = hashmap_init();
  keywords_set("and",   AND);
  keywords_set("class", CLASS);
  keywords_set("and",   AND);
  keywords_set("class", CLASS);
  keywords_set("else",  ELSE);
  keywords_set("false", FALSE);
  keywords_set("for",   FOR);
  keywords_set("fun",   FUN);
  keywords_set("if",    IF);
  keywords_set("nil",   NIL);
  keywords_set("or",    OR);
  keywords_set("print", PRINT);
  keywords_set("return",RETURN);
  keywords_set("super", SUPER);
  keywords_set("this",  THIS);
  keywords_set("true",  TRUE);
  keywords_set("var",   VAR);
  keywords_set("while", WHILE);
}

Token** scanner_scan() {
  while (!isfinished()) {
    lox.scanner.start = lox.scanner.current;
    scan_token();
  }
  add_token(SOURCE_END, NULL);
  hashmap_free(keywords);
  return lox.scanner.tokens;
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
      lox.scanner.line++;
      lox.scanner.last_line = lox.scanner.current - 1;
      break;
    default:
      if (isdigit(c)) {
        scan_number();
      } else if (isalpha(c)) {
        scan_identifier();
      } else {
        report("Got unexpected character.", fileloc_init(lox.scanner.line, lox.scanner.current - 1, lox.scanner.current -1));
      }
      break;
  }

}

static void add_token(TokenType type, LoxObject* literal) {
  char* text = substring(lox.scanner.source, lox.scanner.start, lox.scanner.current-1); // current points to the next "char" so we subtract by 1
  FileLoc* fl = fileloc_init(lox.scanner.line, lox.scanner.start, lox.scanner.current);
  Token* token = token_init(type, text, literal, fl);

  // Reallocate memory
  if (lox.scanner.parsed == lox.scanner.capacity) {
    lox.scanner.capacity = 2 * lox.scanner.capacity;
    lox.scanner.tokens = realloc(lox.scanner.tokens, lox.scanner.capacity * sizeof(Token*));
  }

  lox.scanner.tokens[lox.scanner.parsed] = token;
  lox.scanner.parsed++;
}

static char advance() {
  if (isfinished()) return '\0';

  lox.scanner.current++;
  return lox.scanner.source[lox.scanner.current - 1];
}

static void scan_number() {

  while (isdigit(peek())) 
    advance();

  
  if (peek() == '.') {
    advance();
    while (isdigit(peek())) 
      advance();
  } 
  char* num_lexeme = substring(lox.scanner.source, lox.scanner.start, lox.scanner.current-1);
  FileLoc* fl = fileloc_init(lox.scanner.line, lox.scanner.start, lox.scanner.current - 1);
  LoxObject* literal = loxobject_init(LOX_NUMBER, num_lexeme, fl);

  add_token(NUMBER, literal);

}

static void scan_string() {
  while (peek() != '"' && !isfinished()) {
    if (peek() == '\n') lox.scanner.line++;
    advance();
  }

  // handle when the string doesn't terminate
  if (isfinished()) {
    report("Expected a closing \" at this position.", fileloc_init(lox.scanner.line, lox.scanner.current-1, lox.scanner.current-1));
    return;
  }
  // consume the last '"' character
  advance();

  char* text = substring(lox.scanner.source, lox.scanner.start+1, lox.scanner.current-2);
  FileLoc* fl = fileloc_init(lox.scanner.line, lox.scanner.start, lox.scanner.current - 1);

  LoxObject* literal = loxobject_init(LOX_STRING, text, fl);
  add_token(STRING, literal); 
}

static void scan_identifier() {
  while (isalnum(peek()))
    advance();

  char* identifier = substring(lox.scanner.source, lox.scanner.start, lox.scanner.current-1);
  TokenType type = keywords_match(identifier);

  add_token(type, NULL);
}


static bool isfinished() {
  return lox.scanner.current >= strlen(lox.scanner.source);
}

static char peek() {
  if (isfinished()) return '\0';
  
  return lox.scanner.source[lox.scanner.current];
}

static char peek_next() {
  if (isfinished()) return '\0';
  
  return lox.scanner.source[lox.scanner.current+1];
}

static bool match(char expected) {
  // printf("Got: %c, Expected: %c \n", scanner_peek(scanner), expected);
  if (peek() == expected) {
    lox.scanner.current++;
    return true;
  }
  return false;
}


static TokenType keywords_match(char* text) {
  void* retval = hashmap_retrieve(keywords, text);
  if (retval == NULL) return IDENTIFIER;

  TokenType token =  *(TokenType* ) retval;
  return token;
}

static void keywords_set(char* text, TokenType type) {
  TokenType* type_ptr = malloc(1 * sizeof(TokenType));
  *type_ptr = type;

  // printf("INSERTING: %d\n", *(TokenType*) type_ptr);

  hashmap_insert(keywords, text, type_ptr);
}



