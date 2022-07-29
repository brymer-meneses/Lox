use crate::source_location::SourceLocation;
use crate::{object::LoxObject, token::Token};

#[derive(Debug, PartialEq, Clone)]
pub enum Expr {
    Binary {
        location: SourceLocation,
        left: Box<Expr>,
        operator: Token,
        right: Box<Expr>,
    },
    Unary {
        location: SourceLocation,
        operator: Token,
        right: Box<Expr>,
    },
    Literal {
        location: SourceLocation,
        literal: LoxObject,
    },
    Grouping {
        location: SourceLocation,
        expression: Box<Expr>,
    },
}

#[derive(Debug, PartialEq, Clone)]
pub enum Stmt {
    Block {
        location: SourceLocation,
        statements: Vec<Stmt>,
    },
    Expression {
        location: SourceLocation,
        expression: Expr,
    },
    Print {
        location: SourceLocation,
        expression: Expr,
    }
}

impl Stmt {
    pub fn location(&self) -> SourceLocation {
        match &self {
            Stmt::Block { location, .. } => *location,
            Stmt::Expression { location, .. } => *location,
            Stmt::Print { location, .. } => *location,
        }
    }
}
impl Expr {
    pub fn location(&self) -> SourceLocation {
        match &self {
            Expr::Binary { location, .. } => *location,
            Expr::Unary { location, .. } => *location,
            Expr::Literal { location, .. } => *location,
            Expr::Grouping { location, .. } => *location,
        }
    }
}

pub trait StmtVisitor<T> {
    fn visit_statement(&self, statement: &Stmt) -> T;
    fn visit_block_statement(&self, statement: &Vec<Stmt>) -> T;
    fn visit_expression_statement(&self, statement: &Stmt) -> T;
    fn visit_print_statement(&self, expression: &Expr) -> T;
}

pub trait ExpressionVisitor<T> {
    fn visit_expression(&self, expression: &Expr) -> T;
    fn visit_grouping_expression(&self, expression: &Expr) -> T;
    fn visit_binary_expression(&self, left: &Expr, token: &Token, right: &Expr) -> T;
    fn visit_unary_expression(&self, operator: &Token, right: &Expr) -> T;
    fn visit_literal_expression(&self, literal: LoxObject) -> T;
}
