use crate::ast::Stmt;
use crate::{function::Function, source_location::SourceLocation};
use std::fmt::Display;

#[derive(Debug, Clone, PartialEq)]
pub enum LoxObject {
    String {
        location: SourceLocation,
        value: String,
    },
    Number {
        location: SourceLocation,
        value: f64,
    },
    Boolean {
        location: SourceLocation,
        value: bool,
    },
    Nil {
        location: SourceLocation,
    },
    Callable {
        function: Function,
    },
}

impl LoxObject {
    pub fn location(&self) -> SourceLocation {
        match &self {
            LoxObject::String { location, .. } => *location,
            LoxObject::Number { location, .. } => *location,
            LoxObject::Boolean { location, .. } => *location,
            LoxObject::Nil { location, .. } => *location,
            LoxObject::Callable { function } => todo!(),
        }
    }

    pub fn type_to_string(&self) -> String {
        match self {
            LoxObject::String { .. } => "string".to_string(),
            LoxObject::Number { .. } => "number".to_string(),
            LoxObject::Boolean { .. } => "boolean".to_string(),
            LoxObject::Nil { .. } => "nil".to_string(),
            LoxObject::Callable { .. } => "callable".to_string(),
        }
    }

    pub fn is_truthy(&self) -> bool {
        match self {
            LoxObject::Number { value, .. } => {
                if *value == 0.0 {
                    return false;
                }
                true
            }
            LoxObject::Boolean { value, .. } => *value,
            LoxObject::String { .. } => true,
            LoxObject::Callable { .. } => true,
            LoxObject::Nil { .. } => false,
        }
    }

    pub fn is_equal(left: LoxObject, right: LoxObject) -> bool {
        match (left, right) {
            (LoxObject::String { value: val1, .. }, LoxObject::String { value: val2, .. }) => {
                val1 == val2
            }
            (LoxObject::Number { value: val1, .. }, LoxObject::Number { value: val2, .. }) => {
                val1 == val2
            }
            (LoxObject::Boolean { value: val1, .. }, LoxObject::Boolean { value: val2, .. }) => {
                val1 == val2
            }
            (LoxObject::Nil { .. }, LoxObject::Nil { .. }) => true,
            _ => false,
        }
    }
    pub fn define_user_function(function: &Stmt) -> Self {
        let (name, params, body) = match function {
            Stmt::Function {
                name,
                parameters,
                body,
                ..
            } => (name, parameters, body),
            _ => panic!("Expected function statement."),
        };

        let body = match body.as_ref() {
            Stmt::Block { statements, .. } => statements,
            _ => panic!("Expected block statement as a body for a function."),
        };

        LoxObject::Callable {
            function: Function::User {
                name: Box::new(name.clone()),
                params: params.to_vec(),
                body: body.to_vec(),
            },
        }
    }
}

impl Display for LoxObject {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            LoxObject::String { value, .. } => write!(f, "{}", value),
            LoxObject::Number { value, .. } => write!(f, "{}", value),
            LoxObject::Boolean { value, .. } => write!(f, "{}", value),
            LoxObject::Nil { .. } => write!(f, "nil"),
            LoxObject::Callable { function } => match function {
                Function::User { name, .. } => write!(f, "<user fn {}>", name.lexeme),
                Function::Native { .. } => write!(f, "<builtin fn>"),
            },
        }
    }
}
