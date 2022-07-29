use crate::{object::LoxObject, token::Token};

#[derive(Debug, PartialEq)]
pub enum Expr {
    Bimary(Box<Expr>, Token, Box<Expr>),
    Unary(Token, Box<Expr>),
    Literal(LoxObject),
    Grouping(Box<Expr>)
}

#[derive(Debug, PartialEq)]
pub enum Stmt {
    Block(Vec<Stmt>),
    Expression(Box<Expr>),
}
