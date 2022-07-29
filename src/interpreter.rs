use crate::{ast::{StmtVisitor, ExpressionVisitor, Expr, Stmt}, object::LoxObject, error::InterpreterError, token::TokenType};
use crate::token::Token;

type InterpreterResult<T> = Result<T, InterpreterError>;

pub struct Interpreter {}

impl Interpreter {
    pub fn interpret(&self, statements: &Vec<Stmt>) -> InterpreterResult<()> {
        for statement in statements.iter() {
            self.visit_statement(statement)?;
        }
        Ok(())
    }
    pub fn new() -> Self {
        return Interpreter {};

    }

}

impl ExpressionVisitor<InterpreterResult<LoxObject>> for Interpreter {

    fn visit_expression(&self, expression: &Expr) -> InterpreterResult<LoxObject> {
        match expression {
            Expr::Binary { left, operator, right, .. } => self.visit_binary_expression(&left, &operator, &right),
            Expr::Unary {  operator, right, .. } => self.visit_unary_expression(&operator, &right),
            Expr::Grouping {  expression, .. } => self.visit_grouping_expression(&expression),
            Expr::Literal {  literal, .. } => Ok(literal.clone()),
        }
    }

    fn visit_grouping_expression(&self, expression: &Expr) -> InterpreterResult<LoxObject> {
        return self.visit_expression(expression);
    }
    fn visit_binary_expression(&self, left: &Expr, token: &Token, right: &Expr) -> InterpreterResult<LoxObject> {
        let left = self.visit_expression(left)?;
        let right = self.visit_expression(right)?;
        let location = left.location() + token.location + right.location();

        match token.kind {
            TokenType::Plus => { left + right },
            _ => Err(InterpreterError::InvalidUnaryOperation(location, token.lexeme.clone()))
        }

    }
    fn visit_unary_expression(&self, operator: &Token, right: &Expr) -> InterpreterResult<LoxObject> {
        todo!()
    }
    fn visit_literal_expression(&self, literal: LoxObject) -> InterpreterResult<LoxObject> {
        todo!()
    }
}

impl StmtVisitor<InterpreterResult<LoxObject>> for Interpreter {
    fn visit_statement(&self, statement: &Stmt) -> InterpreterResult<LoxObject> {
        match statement {
            Stmt::Block { statements, .. } => self.visit_block_statement(statements),
            Stmt::Expression { expression, .. } => { self.visit_expression(expression) },
            Stmt::Print { expression, ..} => { self.visit_print_statement(expression)},
        }
    }
    fn visit_block_statement(&self, statements: &Vec<Stmt>) -> InterpreterResult<LoxObject>{
        todo!()
    }
    fn visit_expression_statement(&self, statement: &Stmt) -> InterpreterResult<LoxObject> {
        todo!()
    }
    fn visit_print_statement(&self, expression: &Expr) -> InterpreterResult<LoxObject> {
        let expr = self.visit_expression(expression)?;
        print!("{}", expr);

        Ok(LoxObject::Nil { location: expr.location()})
    }
}


