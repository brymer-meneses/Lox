use crate::object::LoxObject;
use crate::source_location::SourceLocation;
use crate::token::Token;
use std::fmt::Display;

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
    Logical {
        location: SourceLocation,
        left: Box<Expr>,
        operator: Token,
        right: Box<Expr>,
    },
    Grouping {
        location: SourceLocation,
        expression: Box<Expr>,
    },
    Variable {
        location: SourceLocation,
        identifier: Token,
    },
    Assignment {
        location: SourceLocation,
        identifier: Token,
        value: Box<Expr>,
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
    If {
        location: SourceLocation,
        condition: Expr,
        then_branch: Box<Stmt>,
        else_branch: Option<Box<Stmt>>,
    },
}

impl Stmt {
    pub fn location(&self) -> SourceLocation {
        match &self {
            Stmt::Block { location, .. } => *location,
            Stmt::Expression { location, .. } => *location,
            Stmt::Print { location, .. } => *location,
            Stmt::VariableDeclaration { location, .. } => *location,
            Stmt::If { location, .. } => *location,
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
            Expr::Assignment { location, .. } => *location,
            Expr::Logical { location, .. } => *location,
        }
    }
}

impl Display for Expr {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self {
            Expr::Binary {
                left,
                operator,
                right,
                ..
            } => {
                write!(f, "{} {} {}", left, operator.lexeme, right)
            }
            Expr::Unary {
                operator, right, ..
            } => {
                write!(f, "{} {}", operator.lexeme, right)
            }
            Expr::Literal { literal, .. } => {
                write!(f, "{}", literal)
            }
            Expr::Grouping { expression, .. } => {
                write!(f, "{}", expression)
            }
            Expr::Variable { identifier, .. } => {
                write!(f, "{}", identifier.lexeme)
            }
            Expr::Assignment {
                identifier, value, ..
            } => {
                write!(f, "{} = {}", identifier.lexeme, value)
            }
            Expr::Logical {
                left,
                operator,
                right,
                ..
            } => {
                write!(f, "{} {} {}", left, operator.lexeme, right)
            }
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
            Stmt::If {
                condition,
                then_branch,
                else_branch,
                ..
            } => self.visit_if_statement(condition, then_branch, else_branch.as_ref()),
        }
    }

    fn visit_block_statement(&mut self, statement: &[Stmt]) -> T;
    fn visit_expression_statement(&mut self, expression: &Expr) -> T;
    fn visit_print_statement(&mut self, expression: &Expr) -> T;
    fn visit_variable_declaration_statement(
        &mut self,
        identifer: &Token,
        value: &Option<Expr>,
    ) -> T;
    fn visit_if_statement(
        &mut self,
        condition: &Expr,
        then_branch: &Stmt,
        else_branch: Option<&Box<Stmt>>,
    ) -> T;
}

pub trait ExpressionVisitor<T> {
    fn evaluate(&mut self, expression: &Expr) -> T {
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
            Expr::Logical {
                left,
                operator,
                right,
                ..
            } => self.visit_logical_expression(left, operator, right),
            Expr::Grouping { expression, .. } => self.visit_grouping_expression(expression),
            Expr::Literal { literal, .. } => self.visit_literal_expression(literal.clone()),
            Expr::Variable { identifier, .. } => self.visit_variable_expression(identifier),
            Expr::Assignment {
                identifier, value, ..
            } => self.visit_assignment_expression(identifier, value),
        }
    }
    fn visit_grouping_expression(&mut self, expression: &Expr) -> T;
    fn visit_variable_expression(&self, expression: &Token) -> T;
    fn visit_binary_expression(&mut self, left: &Expr, token: &Token, right: &Expr) -> T;
    fn visit_unary_expression(&mut self, operator: &Token, right: &Expr) -> T;
    fn visit_logical_expression(&mut self, left: &Expr, token: &Token, right: &Expr) -> T;
    fn visit_literal_expression(&self, literal: LoxObject) -> T;
    fn visit_assignment_expression(&mut self, identifier: &Token, value: &Expr) -> T;
}
