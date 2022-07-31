use std::usize;

use crate::ast::Expr;
use crate::ast::Stmt;
use crate::error::ParserError;
use crate::object::LoxObject;
use crate::source_location::SourceLocation;
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
            let statement = self.parse_declaration()?;
            self.statements.push(statement);
        }

        Ok(&self.statements)
    }
}

impl<'a> Parser<'a> {
    fn parse_statement(&mut self) -> ParserResult<Stmt> {
        if self.match_token(&[TokenType::Print]) {
            return self.parse_print_statement();
        }

        self.parse_expression_statement()
    }

    fn parse_expression_statement(&mut self) -> ParserResult<Stmt> {
        Ok(Stmt::Expression {
            location: self.peek().location,
            expression: self.parse_expression()?,
        })
    }

    fn parse_print_statement(&mut self) -> ParserResult<Stmt> {
        let expression = self.parse_expression()?;
        Ok(Stmt::Print {
            location: self.find_location(TokenType::Print) + expression.location(),
            expression,
        })
    }

    fn parse_declaration(&mut self) -> ParserResult<Stmt> {
        if self.match_token(&[TokenType::Var]) {
            return self.parse_variable_declaration();
        }

        self.parse_statement()
    }

    fn parse_variable_declaration(&mut self) -> ParserResult<Stmt> {
        let identifier = self.expect(
            TokenType::Identifier,
            ParserError::ExpectedVariableName(self.peek().location),
        )?;

        let expression = match self.match_token(&[TokenType::Equal]) {
            true => Some(self.parse_expression()?),
            false => None,
        };

        self.expect(
            TokenType::Semicolon,
            ParserError::ExpectedToken(self.peek().location, ";".to_string()),
        )?;

        Ok(Stmt::VariableDeclaration {
            location: identifier.location,
            identifier: identifier.clone(),
            expression,
        })
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
                location: expr.location() + operator.location + right.location(),
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
                location: expr.location() + operator.location + right.location(),
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
                location: expr.location() + operator.location + right.location(),
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
                location: expr.location() + operator.location + right.location(),
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
                location: operator.location + right.location(),
                operator: operator.clone(),
                right: Box::new(right),
            });
        }

        self.parse_primary()
    }

    fn parse_primary(&mut self) -> ParserResult<Expr> {
        if self.match_token(&[TokenType::False]) {
            let location = self.previous().location;
            return Ok(Expr::Literal {
                location,
                literal: LoxObject::Boolean {
                    location,
                    value: false,
                },
            });
        }

        if self.match_token(&[TokenType::True]) {
            let location = self.previous().location;
            return Ok(Expr::Literal {
                location,
                literal: LoxObject::Boolean {
                    location,
                    value: true,
                },
            });
        }

        if self.match_token(&[TokenType::Nil]) {
            let location = self.previous().location;
            return Ok(Expr::Literal {
                location,
                literal: LoxObject::Nil { location },
            });
        }

        if self.match_token(&[TokenType::String, TokenType::Number]) {
            let location = self.previous().location;
            return Ok(Expr::Literal {
                location,
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
                location: self.find_location(TokenType::LeftParen)
                    + expr.location()
                    + self.find_location(TokenType::RightParen),
                expression: Box::new(expr),
            });
        }

        if self.match_token(&[TokenType::Identifier]) {
            return Ok(Expr::Variable {
                location: self.previous().location,
                identifier: self.previous().clone(),
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

    fn find_location(&self, expected: TokenType) -> SourceLocation {
        for i in (0..self.current).rev() {
            if self.tokens[i].kind == expected {
                return self.tokens[i].location;
            }
        }
        panic!("Cannot find last occurence of {:?}", expected);
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