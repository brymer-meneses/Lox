use crate::environment::Environment;
use crate::token::Token;
use crate::{
    ast::{Expr, ExpressionVisitor, Stmt, StmtVisitor},
    error::InterpreterError,
    object::LoxObject,
    token::TokenType,
};

pub type InterpreterResult<T> = Result<T, InterpreterError>;

pub struct Interpreter {
    environment: Environment,
}

impl Interpreter {
    pub fn interpret(&mut self, statements: &[Stmt]) -> InterpreterResult<()> {
        for statement in statements.iter() {
            self.execute(statement)?;
        }
        Ok(())
    }
    pub fn new(environment: Option<Environment>) -> Self {
        Interpreter {
            environment: Environment::new(environment),
        }
    }
}

impl ExpressionVisitor<InterpreterResult<LoxObject>> for Interpreter {
    fn visit_grouping_expression(&self, expression: &Expr) -> InterpreterResult<LoxObject> {
        self.evaluate(expression)
    }
    fn visit_binary_expression(
        &self,
        left: &Expr,
        token: &Token,
        right: &Expr,
    ) -> InterpreterResult<LoxObject> {
        let left = self.evaluate(left)?;
        let right = self.evaluate(right)?;
        let location = left.location() + token.location + right.location();
        let error = InterpreterError::InvalidBinaryOperation(
            location,
            left.to_string(),
            token.lexeme.clone(),
            right.to_string(),
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
            _ => Err(error),
        }
    }
    fn visit_unary_expression(
        &self,
        operator: &Token,
        right: &Expr,
    ) -> InterpreterResult<LoxObject> {
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

        Err(InterpreterError::InvalidUnaryOperation(
            operator.location + right.location(),
            operator.lexeme.clone(),
            right.type_to_string(),
        ))
    }
    fn visit_literal_expression(&self, literal: LoxObject) -> InterpreterResult<LoxObject> {
        Ok(literal)
    }

    fn visit_variable_expression(&self, identifier: &Token) -> InterpreterResult<LoxObject> {
        let value = match self.environment.retrieve(identifier.lexeme.as_str()) {
            Some(value) => Ok(value.clone()),
            None => Err(InterpreterError::UndefinedVariable(
                identifier.location,
                identifier.lexeme.clone(),
            )),
        };

        value
    }
}

impl StmtVisitor<InterpreterResult<()>> for Interpreter {
    fn visit_block_statement(&mut self, statements: &Vec<Stmt>) -> InterpreterResult<()> {
        for statement in statements.iter() {
            self.execute(statement)?
        }
        Ok(())
    }
    fn visit_expression_statement(&self, expression: &Expr) -> InterpreterResult<()> {
        self.evaluate(expression)?;
        Ok(())
    }
    fn visit_print_statement(&self, expression: &Expr) -> InterpreterResult<()> {
        let expr = self.evaluate(expression)?;
        print!("{}", expr);
        Ok(())
    }
    fn visit_variable_declaration_statement(
        &mut self,
        identifier: &Token,
        expression: &Option<Expr>,
    ) -> InterpreterResult<()> {
        let value = match expression {
            Some(expr) => self.evaluate(expr)?,
            None => LoxObject::Nil {
                location: identifier.location,
            },
        };

        self.environment.define(identifier.lexeme.clone(), value);
        Ok(())
    }
}
