use crate::environment::Environment;
use crate::token::Token;
use crate::{
    ast::{Expr, ExpressionVisitor, Stmt, StmtVisitor},
    error::{LoxError, LoxErrorKind},
    object::LoxObject,
    token::TokenType,
};

use std::cell::RefCell;
use std::rc::Rc;

pub struct Interpreter {
    environment: Rc<RefCell<Environment>>,
}

use crate::error::LoxResult;

impl Interpreter {
    pub fn interpret(&mut self, statements: &[Stmt]) -> LoxResult<()> {
        for statement in statements.iter() {
            self.execute(statement)?;
        }
        Ok(())
    }
    pub fn new() -> Self {
        Interpreter {
            environment: Rc::new(RefCell::new(Environment::new(None))),
        }
    }
}

impl ExpressionVisitor<LoxResult<LoxObject>> for Interpreter {
    fn visit_grouping_expression(&mut self, expression: &Expr) -> LoxResult<LoxObject> {
        self.evaluate(expression)
    }
    fn visit_binary_expression(
        &mut self,
        left: &Expr,
        token: &Token,
        right: &Expr,
    ) -> LoxResult<LoxObject> {
        let left = self.evaluate(left)?;
        let right = self.evaluate(right)?;
        let location = left.location() + token.location + right.location();
        let error = LoxError::new(
            LoxErrorKind::InvalidBinaryOperation {
                left: left.to_string(),
                operator: token.lexeme.to_owned(),
                right: right.to_string(),
            },
            location,
        );

        match token.kind {
            TokenType::Plus => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Number {
                        location,
                        value: val1 + val2,
                    });
                }
                if let (
                    LoxObject::String { value: val1, .. },
                    LoxObject::String { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::String {
                        location,
                        value: val1.to_owned() + val2,
                    });
                }
                Err(error)
            }
            TokenType::Minus => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Number {
                        location,
                        value: val1 - val2,
                    });
                }
                Err(error)
            }
            TokenType::Star => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Number {
                        location,
                        value: val1 * val2,
                    });
                }
                Err(error)
            }
            TokenType::Slash => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Number {
                        location,
                        value: val1 / val2,
                    });
                }
                Err(error)
            }
            TokenType::EqualEqual => Ok(LoxObject::Boolean {
                location,
                value: LoxObject::is_equal(left, right),
            }),
            TokenType::BangEqual => Ok(LoxObject::Boolean {
                location,
                value: !LoxObject::is_equal(left, right),
            }),
            TokenType::GreaterEqual => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Boolean {
                        location,
                        value: val1 >= val2,
                    });
                }
                Err(error)
            }
            TokenType::Greater => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Boolean {
                        location,
                        value: val1 > val2,
                    });
                }
                Err(error)
            }
            TokenType::LessEqual => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Boolean {
                        location,
                        value: val1 <= val2,
                    });
                }
                Err(error)
            }
            TokenType::Less => {
                if let (
                    LoxObject::Number { value: val1, .. },
                    LoxObject::Number { value: val2, .. },
                ) = (&left, &right)
                {
                    return Ok(LoxObject::Boolean {
                        location,
                        value: val1 < val2,
                    });
                }
                Err(error)
            }
            _ => Err(error),
        }
    }
    fn visit_unary_expression(&mut self, operator: &Token, right: &Expr) -> LoxResult<LoxObject> {
        let right = self.evaluate(right)?;

        if let (TokenType::Bang, LoxObject::Boolean { value, location }) = (&operator.kind, &right)
        {
            return Ok(LoxObject::Boolean {
                location: *location,
                value: !value,
            });
        }

        if let (TokenType::Minus, LoxObject::Number { value, location }) = (&operator.kind, &right)
        {
            return Ok(LoxObject::Number {
                location: *location,
                value: -value,
            });
        }

        Err(LoxError::new(
            LoxErrorKind::InvalidUnaryOperation {
                operator: operator.lexeme.clone(),
                right: right.type_to_string(),
            },
            operator.location + right.location(),
        ))
    }
    fn visit_literal_expression(&self, literal: LoxObject) -> LoxResult<LoxObject> {
        Ok(literal)
    }

    fn visit_variable_expression(&self, identifier: &Token) -> LoxResult<LoxObject> {
        let value = match self
            .environment
            .borrow()
            .retrieve(identifier.lexeme.as_str())
        {
            Some(value) => Ok(value),
            None => Err(LoxError::new(
                LoxErrorKind::UndefinedVariable {
                    variable: identifier.lexeme.clone(),
                },
                identifier.location,
            )),
        };

        value
    }
    fn visit_assignment_expression(
        &mut self,
        identifier: &Token,
        value: &Expr,
    ) -> LoxResult<LoxObject> {
        let value = self.evaluate(value)?;

        self.environment
            .borrow_mut()
            .assign(identifier.lexeme.clone(), value.clone());

        Ok(value)
    }
    fn visit_logical_expression(
        &mut self,
        left: &Expr,
        token: &Token,
        right: &Expr,
    ) -> LoxResult<LoxObject> {
        let left = self.evaluate(left)?;
        if token.kind == TokenType::Or {
            if left.is_truthy() {
                return Ok(left);
            };
        } else {
            if !left.is_truthy() {
                return Ok(left);
            };
        }

        Ok(self.evaluate(right)?)
    }
}

impl StmtVisitor<LoxResult<()>> for Interpreter {
    fn visit_block_statement(&mut self, statements: &[Stmt]) -> LoxResult<()> {
        let previous_environment = self.environment.clone();

        self.environment = Environment::wrap(self.environment.clone());

        for statement in statements.iter() {
            self.execute(statement)?
        }

        self.environment = previous_environment;
        Ok(())
    }
    fn visit_expression_statement(&mut self, expression: &Expr) -> LoxResult<()> {
        self.evaluate(expression)?;
        Ok(())
    }
    fn visit_print_statement(&mut self, expression: &Expr) -> LoxResult<()> {
        let expr = self.evaluate(expression)?;
        println!("{}", expr);
        Ok(())
    }
    fn visit_variable_declaration_statement(
        &mut self,
        identifier: &Token,
        expression: &Option<Expr>,
    ) -> LoxResult<()> {
        let value = match expression {
            Some(expr) => self.evaluate(expr)?,
            None => LoxObject::Nil {
                location: identifier.location,
            },
        };

        self.environment
            .borrow_mut()
            .define(identifier.lexeme.clone(), value);
        Ok(())
    }
    fn visit_if_statement(
        &mut self,
        condition: &Expr,
        then_branch: &Stmt,
        else_branch: Option<&Box<Stmt>>,
    ) -> LoxResult<()> {
        if self.evaluate(condition)?.is_truthy() {
            self.execute(then_branch)?;
        } else if else_branch.is_some() {
            self.execute(&else_branch.unwrap())?;
        }

        Ok(())
    }
}
