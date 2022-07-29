use crate::error::ParserError;
use crate::object::LoxObject;
use crate::syntax::Expr;
use crate::syntax::Stmt;
use crate::token::{Token, TokenType};

pub struct Parser<'a> {
    current: usize,
    tokens: &'a Vec<Token>,
    statements: Vec<Stmt>,
}

type ParserResult<T> = Result<T, ParserError>;

// API
impl<'a> Parser<'a> {
    pub fn new(tokens: &'a Vec<Token>) -> Self {
        Parser {
            current: 0,
            tokens,
            statements: Vec::new(),
        }
    }
    pub fn parse(&mut self) -> ParserResult<&Vec<Stmt>> {
        while !self.is_at_end() {
            let statement = self.parse_declaration_statement()?;
            self.statements.push(statement);
        }

        Ok(&self.statements)
    }
}

impl<'a> Parser<'a> {
    fn parse_expression_statement(&mut self) -> ParserResult<Stmt> {
        Ok(Stmt::Expression {
            location: self.peek().location,
            expression: self.parse_expression()?,
        })
    }

    fn parse_declaration_statement(&mut self) -> ParserResult<Stmt> {
        self.parse_expression_statement()
    }
}

// parsing expressions
impl<'a> Parser<'a> {
    fn parse_expression(&mut self) -> ParserResult<Expr> {
        self.parse_equality()
    }

    fn parse_assignment(&mut self) -> ParserResult<Expr> {
        todo!()
    }

    fn parse_equality(&mut self) -> ParserResult<Expr> {
        let mut expr = self.parse_comparison()?;

        while self.match_token(&[TokenType::BangEqual, TokenType::EqualEqual]) {
            let operator = self.previous();
            let right = self.parse_comparison()?;

            expr = Expr::Binary {
                location: expr.location(),
                left: Box::new(expr),
                operator: operator.clone(),
                right: Box::new(right),
            }
        }

        Ok(expr)
    }

    fn parse_comparison(&mut self) -> ParserResult<Expr> {
        let mut expr = self.parse_term()?;

        while self.match_token(&[
            TokenType::Greater,
            TokenType::GreaterEqual,
            TokenType::Less,
            TokenType::LessEqual,
        ]) {
            let operator = self.previous();
            let right = self.parse_term()?;

            expr = Expr::Binary {
                location: self.previous().location,
                left: Box::new(expr),
                operator: operator.clone(),
                right: Box::new(right),
            }
        }

        Ok(expr)
    }

    fn parse_term(&mut self) -> ParserResult<Expr> {
        let mut expr = self.parse_factor()?;

        while self.match_token(&[TokenType::Minus, TokenType::Plus]) {
            let operator = self.previous();
            let right = self.parse_factor()?;

            expr = Expr::Binary {
                location: self.previous().location,
                left: Box::new(expr),
                operator: operator.clone(),
                right: Box::new(right),
            }
        }

        Ok(expr)
    }
    fn parse_factor(&mut self) -> ParserResult<Expr> {
        let mut expr = self.parse_unary()?;

        while self.match_token(&[TokenType::Slash, TokenType::Star]) {
            let operator = self.previous();
            let right = self.parse_unary()?;

            expr = Expr::Binary {
                location: self.previous().location,
                left: Box::new(expr),
                operator: operator.clone(),
                right: Box::new(right),
            }
        }

        Ok(expr)
    }
    fn parse_unary(&mut self) -> ParserResult<Expr> {
        if self.match_token(&[TokenType::Bang, TokenType::Minus]) {
            let operator = self.previous();
            let right = self.parse_unary()?;

            return Ok(Expr::Unary {
                location: self.peek().location,
                operator: operator.clone(),
                right: Box::new(right),
            });
        }

        self.parse_primary()
    }

    fn parse_primary(&mut self) -> ParserResult<Expr> {
        if self.match_token(&[TokenType::False]) {
            return Ok(Expr::Literal {
                location: self.previous().location,
                literal: LoxObject::Boolean(false),
            });
        }

        if self.match_token(&[TokenType::True]) {
            return Ok(Expr::Literal {
                location: self.previous().location,
                literal: LoxObject::Boolean(true),
            });
        }

        if self.match_token(&[TokenType::Nil]) {
            return Ok(Expr::Literal {
                location: self.previous().location,
                literal: LoxObject::Nil,
            });
        }

        if self.match_token(&[TokenType::String, TokenType::Number]) {
            return Ok(Expr::Literal {
                location: self.previous().location,
                literal: self.previous().literal.as_ref().unwrap().clone(),
            });
        }

        if self.match_token(&[TokenType::LeftParen]) {
            let expr = self.parse_expression()?;
            self.expect(
                TokenType::RightParen,
                ParserError::ExpectedToken(self.peek().location, ")".to_string()),
            )?;

            return Ok(Expr::Grouping {
                location: self.previous().location,
                expression: Box::new(expr),
            });
        }

        Err(ParserError::UnexpectedToken(
            self.peek().location,
            self.peek().lexeme.clone(),
        ))
    }
}

// utility functions
impl<'a> Parser<'a> {
    fn match_token(&mut self, types: &[TokenType]) -> bool {
        for expected in types.iter() {
            if self.check(expected) {
                self.advance();
                return true;
            };
        }
        false
    }

    fn expect(&mut self, expected: TokenType, error: ParserError) -> ParserResult<&'a Token> {
        if self.check(&expected) {
            return Ok(self.advance());
        };
        Err(error)
    }

    fn check(&self, expected: &TokenType) -> bool {
        if self.is_at_end() {
            return false;
        };

        self.peek().kind == *expected
    }

    fn advance(&mut self) -> &'a Token {
        if !self.is_at_end() {
            self.current += 1;
        }
        self.previous()
    }

    fn peek(&self) -> &'a Token {
        &self.tokens[self.current]
    }

    fn previous(&self) -> &'a Token {
        if self.current == 0 {
            return &self.tokens[0];
        }
        &self.tokens[self.current - 1]
    }

    fn is_at_end(&self) -> bool {
        self.peek().kind == TokenType::EOF
    }
}

#[cfg(test)]
mod internal_tests {
    use crate::source_location::SourceLocation;

    use super::*;

    #[test]
    fn it_should_match_token() {
        let dummy = SourceLocation::new_single_line(1, 0, 0);
        let tokens = vec![
            Token::new(TokenType::LeftBrace, dummy.clone(), "".to_string(), None),
            Token::new(TokenType::RightBrace, dummy.clone(), "".to_string(), None),
            Token::new(TokenType::Number, dummy.clone(), "".to_string(), None),
        ];
        let mut parser = Parser::new(&tokens);

        assert!(parser.match_token(&[TokenType::LeftBrace]));
        assert!(parser.match_token(&[TokenType::RightBrace]));
        assert!(parser.match_token(&[TokenType::Number]));
    }
}
