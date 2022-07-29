use crate::source_location::SourceLocation;
use crate::{object::LoxObject, token::Token};

#[derive(Debug, PartialEq)]
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

#[derive(Debug, PartialEq)]
pub enum Stmt {
    Block {
        location: SourceLocation,
        statements: Vec<Stmt>,
    },
    Expression {
        location: SourceLocation,
        expression: Expr,
    },
}

impl Stmt {
    pub fn location(&self) -> SourceLocation {
        match &self {
            Stmt::Block { location, .. } => *location,
            Stmt::Expression { location, .. } => *location,
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
