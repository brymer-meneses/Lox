use crate::object::LoxObject;
use crate::source_location::SourceLocation;
use crate::token::Token;

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
    Variable {
        location: SourceLocation,
        identifier: Token,
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
    },
    VariableDeclaration {
        location: SourceLocation,
        identifier: Token,
        expression: Option<Expr>,
    },
}

impl Stmt {
    pub fn location(&self) -> SourceLocation {
        match &self {
            Stmt::Block { location, .. } => *location,
            Stmt::Expression { location, .. } => *location,
            Stmt::Print { location, .. } => *location,
            Stmt::VariableDeclaration { location, .. } => *location,
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
            Expr::Variable { location, .. } => *location,
        }
    }
}

pub trait StmtVisitor<T> {
    fn execute(&mut self, statement: &Stmt) -> T {
        match statement {
            Stmt::Block { statements, .. } => self.visit_block_statement(statements),
            Stmt::Expression { expression, .. } => self.visit_expression_statement(expression),
            Stmt::Print { expression, .. } => self.visit_print_statement(expression),
            Stmt::VariableDeclaration {
                identifier,
                expression,
                ..
            } => self.visit_variable_declaration_statement(identifier, expression),
        }
    }
    fn visit_block_statement(&mut self, statement: &Vec<Stmt>) -> T;
    fn visit_expression_statement(&self, expression: &Expr) -> T;
    fn visit_print_statement(&self, expression: &Expr) -> T;
    fn visit_variable_declaration_statement(
        &mut self,
        identifer: &Token,
        value: &Option<Expr>,
    ) -> T;
}

pub trait ExpressionVisitor<T> {
    fn evaluate(&self, expression: &Expr) -> T {
        match expression {
            Expr::Binary {
                left,
                operator,
                right,
                ..
            } => self.visit_binary_expression(left, operator, right),
            Expr::Unary {
                operator, right, ..
            } => self.visit_unary_expression(operator, right),
            Expr::Grouping { expression, .. } => self.visit_grouping_expression(expression),
            Expr::Literal { literal, .. } => self.visit_literal_expression(literal.clone()),
            Expr::Variable { identifier, .. } => self.visit_variable_expression(identifier),
        }
    }
    fn visit_grouping_expression(&self, expression: &Expr) -> T;
    fn visit_variable_expression(&self, expression: &Token) -> T;
    fn visit_binary_expression(&self, left: &Expr, token: &Token, right: &Expr) -> T;
    fn visit_unary_expression(&self, operator: &Token, right: &Expr) -> T;
    fn visit_literal_expression(&self, literal: LoxObject) -> T;
}
