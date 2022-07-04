#include "lox/object.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "assert.h"

#include "tools/error.h"
#include "tools/hashtable.h"
#include "tools/utils.h"

#include "lox/filelocation.h"
#include "lox/token.h"
#include "lox/scanner.h"
#include "lox/declarations.h"
#include "lox/lox.h"


static TokenType get_keyword(const char* text);

static void register_token(Token token);
static void add_token(TokenType type, LoxObject literal);
static char peek();
static bool match(char expected);
static char advance();
static bool isfinished();

static void scan_token();
static void scan_identifier();
static void scan_string();
static void scan_number();

static HashTable ht;

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
  ht = ht_init();

  ht_insert(&ht, "and",   CAST(AND));
  ht_insert(&ht, "class", CAST(CLASS));
  ht_insert(&ht, "and",   CAST(AND));
  ht_insert(&ht, "class", CAST(CLASS));
  ht_insert(&ht, "else",  CAST(ELSE));
  ht_insert(&ht, "false", CAST(FALSE));
  ht_insert(&ht, "for",   CAST(FOR));
  ht_insert(&ht, "fun",   CAST(FUN));
  ht_insert(&ht, "if",    CAST(IF));
  ht_insert(&ht, "nil",   CAST(NIL));
  ht_insert(&ht, "or",    CAST(OR));
  ht_insert(&ht, "print", CAST(PRINT));
  ht_insert(&ht, "return",CAST(RETURN));
  ht_insert(&ht, "super", CAST(SUPER));
  ht_insert(&ht, "this",  CAST(THIS));
  ht_insert(&ht, "true",  CAST(TRUE));
  ht_insert(&ht, "var",   CAST(VAR));
  ht_insert(&ht, "while", CAST(WHILE));
}

Token* scanner_scan() {
  while (!isfinished()) {
    lox.scanner.start = lox.scanner.current;
    scan_token();
  }
  add_token(SOURCE_END, LOX_OBJECT_NULL);
  return lox.scanner.tokens;
}

static void scan_token() {
  char c = advance();
  switch (c) {
    case '{':
      add_token(LEFT_BRACE, LOX_OBJECT_NULL);
      break;
    case '}': 
      add_token(RIGHT_BRACE, LOX_OBJECT_NULL);
      break;
    case '(':
      add_token(LEFT_PAREN, LOX_OBJECT_NULL);
      break;
    case ')':
      add_token(RIGHT_PAREN, LOX_OBJECT_NULL);
      break;
    case ',':
      add_token(COMMA, LOX_OBJECT_NULL);
      break;
    case '.':
      add_token(DOT, LOX_OBJECT_NULL);
      break;
    case '*':
      add_token(STAR, LOX_OBJECT_NULL);
      break;
    case '+':
      add_token(PLUS, LOX_OBJECT_NULL);
      break;
    case '-':
      add_token(MINUS, LOX_OBJECT_NULL);
      break;
    case ';':
      add_token(SEMICOLON, LOX_OBJECT_NULL);
      break;

    case '>':
      if (match('=')) 
        add_token(GREATER_EQUAL, LOX_OBJECT_NULL);
      else
        add_token(GREATER, LOX_OBJECT_NULL);
      break;

    case '<':
      if (match('=')) 
        add_token(LESS_EQUAL, LOX_OBJECT_NULL);
      else
        add_token(LESS, LOX_OBJECT_NULL);
      break;

    case '=':
      if (match('=')) 
        add_token(EQUAL_EQUAL, LOX_OBJECT_NULL);
      else 
        add_token(EQUAL, LOX_OBJECT_NULL);
      break;

    case '!':
      if (match('='))
        add_token(BANG_EQUAL, LOX_OBJECT_NULL);
      else 
        add_token(BANG, LOX_OBJECT_NULL);
      break;
    case '/':
      if (match('/')) {
        while(peek() != '\n')
          advance();
      } else {
        add_token(SLASH, LOX_OBJECT_NULL);
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
        raise_unexpected_character_error(c);
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

static void add_token(TokenType type, LoxObject literal) {
  char* text = substring(lox.scanner.source, lox.scanner.start, lox.scanner.current-1); // current points to the next "char" so we subtract by 1
  Token token = token_init(type, text, literal, compute_relative_position());
  register_token(token);
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
  const char* num_lexeme = substring(lox.scanner.source, lox.scanner.start, lox.scanner.current-1);
  LoxObject literal = encode_double(strtod(num_lexeme, NULL));

  add_token(NUMBER, literal);

}

static void scan_string() {
  while (peek() != '"' && !isfinished()) {
    if (peek() == '\n') lox.scanner.line++;
    advance();
  }

  // handle when the string doesn't terminate
  if (isfinished()) {
    raise_unterminated_string_error();
    return;
  }
  // consume the last '"' character
  advance();

 char* text = substring(lox.scanner.source, lox.scanner.start+1, lox.scanner.current-2);
 LoxObject literal = encode_string(text);
  add_token(STRING, literal); 
}

static void scan_identifier(Scanner *s) {
  while (isalnum(peek()))
    advance();

  char* identifier = substring(s->source, s->start, s->current-1);
  TokenType type = get_keyword(identifier);

  add_token(type, LOX_OBJECT_NULL);
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


TokenType get_keyword(const char* text) {
  return atoi(ht_retrieve(&ht, (char*)text));
}


