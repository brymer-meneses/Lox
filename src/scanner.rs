use crate::{
    error::ScannerError,
    object::LoxObject,
    source_location::SourceLocation,
    token::{Token, TokenType},
};

pub struct Scanner {
    source_code: String,
    current: usize,
    start: usize,
    line: usize,
    pub tokens: Vec<Token>,
}

type ScannerResult<T> = Result<T, ScannerError>;

impl Scanner {
    pub fn new(source_code: &str) -> Self {
        Scanner {
            source_code: source_code.to_string(),
            current: 0,
            start: 0,
            line: 1,
            tokens: Vec::new(),
        }
    }

    pub fn scan(&mut self) -> ScannerResult<&Vec<Token>> {
        while !self.is_at_end() {
            self.start = self.current;
            self.scan_token()?
        }

        self.add_token(TokenType::EOF, None);
        Ok(&self.tokens)
    }

    fn scan_token(&mut self) -> ScannerResult<()> {
        let c = self.advance();
        match c {
            ';' => self.add_token(TokenType::Semicolon, None),
            '.' => self.add_token(TokenType::Dot, None),
            '*' => self.add_token(TokenType::Star, None),
            '+' => self.add_token(TokenType::Plus, None),
            '-' => self.add_token(TokenType::Minus, None),
            '(' => self.add_token(TokenType::LeftParen, None),
            ')' => self.add_token(TokenType::RightParen, None),
            '}' => self.add_token(TokenType::RightBrace, None),
            '{' => self.add_token(TokenType::LeftBrace, None),
            '/' => {
                if self.match_char('/') {
                    while self.peek() != '\n' && !self.is_at_end() {
                        self.advance();
                    }
                } else {
                    self.add_token(TokenType::Slash, None)
                }
            }
            '=' => {
                if self.match_char('=') {
                    self.add_token(TokenType::EqualEqual, None)
                } else {
                    self.add_token(TokenType::Equal, None)
                }
            }
            '!' => {
                if self.match_char('=') {
                    self.add_token(TokenType::BangEqual, None)
                } else {
                    self.add_token(TokenType::Bang, None)
                }
            }
            '>' => {
                if self.match_char('=') {
                    self.add_token(TokenType::GreaterEqual, None)
                } else {
                    self.add_token(TokenType::Greater, None)
                }
            }
            '<' => {
                if self.match_char('=') {
                    self.add_token(TokenType::LessEqual, None)
                } else {
                    self.add_token(TokenType::Less, None)
                }
            }
            ' ' | '\t' | '\r' => {}
            '\n' => { 
                if !self.is_at_end() {
                  self.line += 1 };
                },
            '"' => self.scan_string()?,
            _ => {
                if c.is_digit(10) {
                    self.scan_number()?;
                } else if c.is_alphabetic() || c == '_' {
                    self.scan_identifier()?;
                } else {
                    return Err(ScannerError::UnexpectedChar(
                        SourceLocation::new_single_line(self.line, self.start, self.current - 1),
                        c,
                    ));
                }
            }
        }
        Ok(())
    }

    fn scan_number(&mut self) -> ScannerResult<()> {
        while self.peek().is_digit(10) {
            self.advance();
        }

        if self.peek() == '.' && self.peek_next().is_digit(10) {
            self.advance();

            while self.peek().is_digit(10) {
                self.advance();
            }
        }

        let number: f64 = self
            .source_code
            .get(self.start..self.current)
            .unwrap()
            .parse()
            .unwrap();

        self.add_token(TokenType::Number, Some(LoxObject::Number(number)));
        Ok(())
    }

    fn scan_identifier(&mut self) -> ScannerResult<()> {
        while self.peek().is_alphanumeric() || self.peek() == '_' {
            self.advance();
        }

        let keyword = self.source_code.get(self.start..self.current).unwrap();
        let token_type = Token::keyword(keyword);
        self.add_token(token_type, None);
        Ok(())
    }

    fn scan_string(&mut self) -> ScannerResult<()> {
        let line_start = self.line;

        while self.peek() != '"' && !self.is_at_end() {
            if self.peek() == '\n' {
                self.line += 1;
            }
            self.advance();
        }

        let line_end = self.line;

        if self.is_at_end() {
            return Err(ScannerError::UnterminatedString(
                SourceLocation::new_multiple_line(
                    line_start,
                    line_end,
                    self.start,
                    self.current - 1,
                ),
            ));
        }

        // consume last ending "
        self.advance();

        let string = self
            .source_code
            .get(self.start + 1..self.current - 1)
            .unwrap()
            .to_string();

        self.add_token(TokenType::String, Some(LoxObject::String(string)));
        Ok(())
    }

    fn peek(&self) -> char {
        return self.source_code.chars().nth(self.current).unwrap_or('\0');
    }

    fn peek_next(&self) -> char {
        return self
            .source_code
            .chars()
            .nth(self.current + 1)
            .unwrap_or('\0');
    }

    fn advance(&mut self) -> char {
        if self.is_at_end() {
            return '\0';
        };

        self.current += 1;
        return self
            .source_code
            .chars()
            .nth(self.current - 1)
            .unwrap_or('\0');
    }

    fn add_token(&mut self, kind: TokenType, literal: Option<LoxObject>) {

        let lexeme = self
            .source_code
            .get(self.start..self.current)
            .unwrap_or("\0")
            .to_string();

        let location =
            SourceLocation::new_single_line(self.line, self.start, self.current - 1);

        self.tokens
            .push(Token::new(kind, location, lexeme, literal));
    }

    fn is_at_end(&self) -> bool {
        self.current == self.source_code.chars().count()
    }

    fn match_char(&mut self, expected: char) -> bool {
        if self.is_at_end() {
            return false;
        };

        let current_char = self.source_code.chars().nth(self.current).unwrap();

        if current_char != expected {
            return false;
        }
        self.current += 1;
        true
    }
}
