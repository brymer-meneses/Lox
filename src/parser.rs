use std::usize;

use crate::ast::Expr;
use crate::ast::Stmt;
use crate::object::LoxObject;
use crate::source_location::SourceLocation;
use crate::token::{Token, TokenType};

pub struct Parser<'a> {
    current: usize,
    tokens: &'a Vec<Token>,
}

use crate::error::LoxError;
use crate::error::LoxResult;

// API
impl<'a> Parser<'a> {
    pub fn new(tokens: &'a Vec<Token>) -> Self {
        Parser { current: 0, tokens }
    }
    pub fn parse(&mut self) -> LoxResult<Vec<Stmt>> {
        let mut statements = Vec::new();

        while !self.is_at_end() {
            statements.push(self.parse_declaration()?);
        }

        Ok(statements)
    }
}

impl<'a> Parser<'a> {
    fn parse_statement(&mut self) -> LoxResult<Stmt> {
        if self.match_token(&[TokenType::Print]) {
            return self.parse_print_statement();
        }

        if self.match_token(&[TokenType::Return]) {
            return self.parse_return_statement();
        }

        if self.match_token(&[TokenType::LeftBrace]) {
            return self.parse_block_statement();
        }

        if self.match_token(&[TokenType::If]) {
            return self.parse_if_statement();
        }

        if self.match_token(&[TokenType::While]) {
            return self.parse_while_loop_statement();
        }

        if self.match_token(&[TokenType::For]) {
            return self.parse_for_loop_statement();
        }

        self.parse_expression_statement()
    }

    fn parse_if_statement(&mut self) -> LoxResult<Stmt> {
        self.expect(
            TokenType::LeftParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "Expected ( after for loop declaration".to_string(),
            },
        )?;
        let condition = self.parse_expression()?;
        self.expect(
            TokenType::RightParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "Expected ) after condition.".to_string(),
            },
        )?;

        let then_branch = Box::new(self.parse_statement()?);
        let mut else_branch = None;
        if self.match_token(&[TokenType::Else]) {
            else_branch = Some(Box::new(self.parse_statement()?));
        }

        Ok(Stmt::If {
            location: self.peek().location,
            condition,
            then_branch,
            else_branch,
        })
    }

    fn parse_expression_statement(&mut self) -> LoxResult<Stmt> {
        let expression = self.parse_expression()?;
        self.expect(
            TokenType::Semicolon,
            LoxError::ExpectedSemicolon {
                location: self.peek().location,
            },
        )?;

        Ok(Stmt::Expression {
            location: expression.location() + self.previous().location,
            expression,
        })
    }

    fn parse_print_statement(&mut self) -> LoxResult<Stmt> {
        let expression = self.parse_expression()?;
        self.expect(
            TokenType::Semicolon,
            LoxError::ExpectedSemicolon {
                location: self.peek().location,
            },
        )?;
        Ok(Stmt::Print {
            location: self.find_location(TokenType::Print) + expression.location(),
            expression,
        })
    }

    fn parse_return_statement(&mut self) -> LoxResult<Stmt> {
        let _ = self.previous();
        let mut value = None;
        if !self.check(&TokenType::Semicolon) {
            value = Some(self.parse_expression()?);
        }

        self.expect(
            TokenType::Semicolon,
            LoxError::ExpectedSemicolon {
                location: self.peek().location,
            },
        )?;

        Ok(Stmt::Return {
            location: self.find_location(TokenType::Return) + self.previous().location,
            value,
        })
    }

    fn parse_block_statement(&mut self) -> LoxResult<Stmt> {
        let mut statements: Vec<Stmt> = Vec::new();
        let mut locations = self.find_location(TokenType::LeftBrace);

        while !self.check(&TokenType::RightBrace) && !self.is_at_end() {
            let statement = self.parse_declaration()?;
            locations = locations + statement.location();
            statements.push(statement);
        }

        let right_brace = self.expect(
            TokenType::RightBrace,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "Expected { after block statement".to_string(),
            },
        )?;

        // add location of the right brace
        locations = locations + right_brace.location;

        Ok(Stmt::Block {
            location: locations,
            statements,
        })
    }

    fn parse_declaration(&mut self) -> LoxResult<Stmt> {
        if self.match_token(&[TokenType::Function]) {
            return self.parse_function();
        }

        if self.match_token(&[TokenType::Var]) {
            return self.parse_variable_declaration();
        }

        self.parse_statement()
    }

    fn parse_function(&mut self) -> LoxResult<Stmt> {
        let name = self.expect(
            TokenType::Identifier,
            LoxError::InvalidFunctionName {
                location: self.peek().location,
                variable: "".to_string(),
            },
        )?;

        self.expect(
            TokenType::LeftParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "(".to_string(),
            },
        )?;

        let mut parameters = Vec::new();
        if !self.check(&TokenType::RightParen) {
            loop {
                if parameters.len() >= 255 {
                    return Err(LoxError::ExceededMaximumArity {
                        location: name.location,
                    });
                }

                parameters.push(
                    self.expect(
                        TokenType::Identifier,
                        LoxError::InvalidFunctionName {
                            location: self.peek().location,
                            variable: "".to_string(),
                        },
                    )?
                    .clone(),
                );

                if !self.match_token(&[TokenType::Comma]) {
                    break;
                }
            }
        }

        self.expect(
            TokenType::RightParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: ")".to_string(),
            },
        )?;

        self.expect(
            TokenType::LeftBrace,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "{".to_string(),
            },
        )?;

        let body = self.parse_block_statement()?;

        Ok(Stmt::Function {
            location: name.location,
            name: name.to_owned(),
            parameters,
            body: Box::new(body),
        })
    }

    fn parse_variable_declaration(&mut self) -> LoxResult<Stmt> {
        let identifier = self.expect(
            TokenType::Identifier,
            LoxError::InvalidVariableName {
                location: self.peek().location,
                variable: self.peek().lexeme.to_owned(),
            },
        )?;

        let expression = match self.match_token(&[TokenType::Equal]) {
            true => Some(self.parse_expression()?),
            false => None,
        };

        self.expect(
            TokenType::Semicolon,
            LoxError::ExpectedSemicolon {
                location: self.peek().location,
            },
        )?;

        Ok(Stmt::VariableDeclaration {
            location: identifier.location,
            identifier: identifier.clone(),
            expression,
        })
    }
    fn parse_while_loop_statement(&mut self) -> LoxResult<Stmt> {
        self.expect(
            TokenType::LeftParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "Expected ( after while keyword.".to_string(),
            },
        )?;

        let condition = self.parse_expression()?;

        self.expect(
            TokenType::RightParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "Expected ) after the condition.".to_string(),
            },
        )?;

        let body = self.parse_statement()?;

        Ok(Stmt::While {
            location: condition.location() + body.location(),
            condition,
            body: Box::new(body),
        })
    }

    fn parse_for_loop_statement(&mut self) -> LoxResult<Stmt> {
        // for (initializer; condition; increment) body

        // NOTE:
        //   desugar for loop into a while loop
        //   {
        //      initializer
        //      while (condition) body
        //   }
        //
        self.expect(
            TokenType::LeftParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "Expected ( after `for` keyword".to_string(),
            },
        )?;

        // Parse initializer
        let initializer: Option<Stmt>;
        if self.match_token(&[TokenType::Semicolon]) {
            initializer = None;
        } else if self.match_token(&[TokenType::Var]) {
            initializer = Some(self.parse_variable_declaration()?);
        } else {
            initializer = Some(self.parse_expression_statement()?);
        }

        // Parse condition
        let mut condition: Option<Expr> = None;

        if !self.match_token(&[TokenType::Semicolon]) {
            condition = Some(self.parse_expression()?);
        }

        self.expect(
            TokenType::Semicolon,
            LoxError::ExpectedSemicolon {
                location: self.peek().location,
            },
        )?;

        // Parse increment
        let mut increment: Option<Expr> = None;
        if !self.match_token(&[TokenType::RightParen]) {
            increment = Some(self.parse_expression()?);
        }

        self.expect(
            TokenType::RightParen,
            LoxError::ExpectedToken {
                location: self.peek().location,
                message: "Expected )".to_string(),
            },
        )?;

        let mut body = self.parse_statement()?;

        if increment.is_some() {
            let location = body.location() + increment.as_ref().unwrap().location();
            body = Stmt::Block {
                location,
                statements: vec![
                    body,
                    Stmt::Expression {
                        location: increment.as_ref().unwrap().location(),
                        expression: increment.unwrap(),
                    },
                ],
            }
        }

        if condition.is_none() {
            let location = condition.as_ref().unwrap().location();
            condition = Some(Expr::Literal {
                location,
                literal: LoxObject::Boolean {
                    location,
                    value: true,
                },
            })
        }
        body = Stmt::While {
            location: body.location(),
            condition: condition.unwrap(),
            body: Box::new(body),
        };

        if initializer.is_some() {
            body = Stmt::Block {
                location: body.location(),
                statements: vec![initializer.unwrap(), body],
            };
        }

        Ok(body)
    }
}

// parsing expressions
impl<'a> Parser<'a> {
    fn parse_expression(&mut self) -> LoxResult<Expr> {
        self.parse_assignment()
    }

    fn parse_assignment(&mut self) -> LoxResult<Expr> {
        let expr = self.parse_or()?;

        if self.match_token(&[TokenType::Equal]) {
            let equals = self.previous();
            let value = self.parse_assignment()?;

            let location = expr.location() + equals.location + value.location();

            if let Expr::Variable { identifier, .. } = expr {
                return Ok(Expr::Assignment {
                    location,
                    identifier,
                    value: Box::new(value),
                });
            }

            return Err(LoxError::InvalidAssignmentTarget {
                location,
                lexeme: expr.to_string(),
            });
        };

        Ok(expr)
    }
    fn parse_or(&mut self) -> LoxResult<Expr> {
        let mut expr = self.parse_and()?;

        while self.match_token(&[TokenType::Or]) {
            let operator = self.previous();
            let right = self.parse_and()?;

            expr = Expr::Logical {
                location: expr.location() + operator.location + right.location(),
                left: Box::new(expr),
                operator: operator.clone(),
                right: Box::new(right),
            }
        }
        Ok(expr)
    }

    fn parse_and(&mut self) -> LoxResult<Expr> {
        let mut expr = self.parse_equality()?;

        while self.match_token(&[TokenType::And]) {
            let operator = self.previous();
            let right = self.parse_equality()?;

            expr = Expr::Logical {
                location: expr.location() + operator.location + right.location(),
                left: Box::new(expr),
                operator: operator.clone(),
                right: Box::new(right),
            }
        }
        Ok(expr)
    }
    fn parse_equality(&mut self) -> LoxResult<Expr> {
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

    fn parse_comparison(&mut self) -> LoxResult<Expr> {
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

    fn parse_term(&mut self) -> LoxResult<Expr> {
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
    fn parse_factor(&mut self) -> LoxResult<Expr> {
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

    fn parse_unary(&mut self) -> LoxResult<Expr> {
        if self.match_token(&[TokenType::Bang, TokenType::Minus]) {
            let operator = self.previous();
            let right = self.parse_unary()?;

            return Ok(Expr::Unary {
                location: operator.location + right.location(),
                operator: operator.clone(),
                right: Box::new(right),
            });
        }

        self.parse_call()
    }

    fn finish_call(&mut self, callee: Expr) -> LoxResult<Expr> {
        let mut arguments: Vec<Expr> = Vec::new();
        let mut location = self.peek().location;

        if !self.check(&TokenType::RightParen) {
            loop {
                let arg = self.parse_expression()?;
                location = location + arg.location();

                if arguments.len() >= 255 {
                    return Err(LoxError::ExceededMaximumArity { location });
                }

                arguments.push(arg);

                if !self.match_token(&[TokenType::Comma]) {
                    break;
                };
            }
        }

        let paren = self
            .expect(
                TokenType::RightParen,
                LoxError::ExpectedToken {
                    location: self.peek().location,
                    message: "Expected after call )".to_string(),
                },
            )?
            .clone();

        Ok(Expr::Call {
            location,
            callee: Box::new(callee),
            paren,
            arguments,
        })
    }

    fn parse_call(&mut self) -> LoxResult<Expr> {
        let mut expr = self.parse_primary()?;

        loop {
            if self.match_token(&[TokenType::LeftParen]) {
                expr = self.finish_call(expr)?;
            } else {
                break;
            }
        }

        Ok(expr)
    }
    fn parse_primary(&mut self) -> LoxResult<Expr> {
        if self.match_token(&[TokenType::False]) {
            let location = self.previous().location;
            return Ok(Expr::Literal {
                location,
                literal: LoxObject::Boolean {
                    location,
                    value: false,
                },
            });
        };

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
            if self.match_token(&[TokenType::RightParen]) {
                let location = self.previous().location;
                return Ok(Expr::Literal {
                    location,
                    literal: LoxObject::Nil { location },
                });
            };

            let expr = self.parse_expression()?;

            self.expect(
                TokenType::RightParen,
                LoxError::ExpectedToken {
                    location: self.peek().location,
                    message: "Expected enclosing ) after grouping expression.".to_string(),
                },
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

        Err(LoxError::ExpectedExpression {
            location: self.peek().location,
        })
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

    fn expect(&mut self, expected: TokenType, error: LoxError) -> LoxResult<&'a Token> {
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
