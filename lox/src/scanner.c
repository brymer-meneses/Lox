#include "lox/filelocation.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "assert.h"

#include "lox/token.h"
#include "lox/scanner.h"
#include "lox/error.h"
#include "lox/utils.h"
#include "lox/lox.h"

static TokenType get_keyword(const char* text);

static void register_token(Token token);
static void add_token(TokenType type);
static char peek();
static bool match(char expected);
static char advance();
static bool isfinished();

static void scan_token();
static void scan_identifier();
static void scan_string();
static void scan_number();


void scanner_init(const char *source) {
  lox.scanner = (Scanner) {
    .current =0, 
    .start =0, 
    .line =1, 
    .source=source,
    .capacity = INITIAL_TOKEN_ARRAY_SIZE,
    .parsed = 0,
    .last_line = 0,
    .tokens = calloc(INITIAL_TOKEN_ARRAY_SIZE, sizeof(Token)),
  };
}

Token* scanner_scan() {
  while (!isfinished()) {
    lox.scanner.start = lox.scanner.current;
    scan_token();
  }
  add_token(SOURCE_END);
  return lox.scanner.tokens;
}

static void scan_token() {
  char c = advance();
  switch (c) {
    case '{':
      add_token(LEFT_BRACE);
      break;
    case '}': 
      add_token(RIGHT_BRACE);
      break;
    case '(':
      add_token(LEFT_PAREN);
      break;
    case ')':
      add_token(RIGHT_PAREN);
      break;
    case ',':
      add_token(COMMA);
      break;
    case '.':
      add_token(DOT);
      break;
    case '*':
      add_token(STAR);
      break;
    case '+':
      add_token(PLUS);
      break;
    case '-':
      add_token(MINUS);
      break;
    case ';':
      add_token(SEMICOLON);
      break;

    case '>':
      if (match('=')) 
        add_token(GREATER_EQUAL);
      else
        add_token(GREATER);
      break;

    case '<':
      if (match('=')) 
        add_token(LESS_EQUAL);
      else
        add_token(LESS);
      break;

    case '=':
      if (match('=')) 
        add_token(EQUAL_EQUAL);
      else 
        add_token(EQUAL);
      break;

    case '!':
      if (match('='))
        add_token(BANG_EQUAL);
      else 
        add_token(BANG);
      break;
    case '/':
      if (match('/')) {
        while(peek() != '\n')
          advance();
      } else {
        add_token(SLASH);
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
        // raise_unexpected_character_error(c);
      }
      break;
  }

}

static void register_token(Token token) {
  if (lox.scanner.parsed == lox.scanner.capacity) {
    lox.scanner.capacity = 2 * lox.scanner.capacity;

    lox.scanner.tokens = realloc(lox.scanner.tokens, lox.scanner.capacity * sizeof(Token));
  }
  lox.scanner.tokens[lox.scanner.parsed] = token;
  lox.scanner.parsed++;
}

void add_token(TokenType type) {
  char* text = substring(lox.scanner.source, lox.scanner.start, lox.scanner.current-1); // current points to the next "char" so we subtract by 1
  Token token = token_init(type, text, compute_relative_position(&lox.scanner));
  register_token(token);
}

static char advance() {
  if (isfinished()) return '\0';

  lox.scanner.current++;
  return lox.scanner.source[lox.scanner.current - 1];
}

static void scan_number(Scanner *s) {

  while (isdigit(peek())) 
    advance();

  
  if (peek() == '.') {
    advance();
    while (isdigit(peek())) 
      advance();
  } 

  add_token(NUMBER);


}

static void scan_string() {
  while (peek() != '"' && !isfinished()) {
    if (peek() == '\n') lox.scanner.line++;
    advance();
  }

  // handle when the string doesn't terminate
  if (isfinished()) {
    raise_unterminated_string_error(substring(lox.scanner.source, lox.scanner.start, lox.scanner.current-1));
    return;
  }
  // consume the last '"' character
  advance();

  add_token(STRING); 
}

void scan_identifier(Scanner *s) {
  while (isalnum(peek()))
    advance();

  char* identifier = substring(s->source, s->start, s->current-1);
  TokenType type = get_keyword(identifier);

  add_token(type);
}


static bool isfinished() {
  return lox.scanner.current >= strlen(lox.scanner.source);
}

static char peek() {
  if (isfinished()) return '\0';
  
  return lox.scanner.source[lox.scanner.current];
}

char peek_next() {
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

char* get_current_line() {
  char** arr = str_split(lox.scanner.source, "\n");
  return arr[0];
}

TokenType get_keyword(const char* text) {
  struct KeywordPair {
    const char* lexeme;
    const TokenType type;
  };

  const struct KeywordPair pairs[] = {
   {"and", AND},
   {"class", CLASS},
   {"else", ELSE},
   {"false", FALSE},
   {"for", FOR},
   {"fun", FUN},
   {"if", IF},
   {"nil", NIL},
   {"or", OR},
   {"print", PRINT},
   {"return", RETURN},
   {"super", SUPER},
   {"this", THIS},
   {"true", TRUE},
   {"var", VAR},
   {"while", WHILE},
  };

  unsigned int length = sizeof(pairs)/sizeof(struct KeywordPair);

  for (int i=0; i<length; i++) {
    if (strcmp(pairs[i].lexeme, text) == 0) {
      return pairs[i].type;
    }
  }
  return IDENTIFIER;
}


