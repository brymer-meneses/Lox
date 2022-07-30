use crate::source_location::SourceLocation;
use std::fmt::Display;

#[derive(Debug, PartialEq, Clone)]
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
}

impl LoxObject {
    pub fn location(&self) -> SourceLocation {
        match &self {
            LoxObject::String { location, .. } => *location,
            LoxObject::Number { location, .. } => *location,
            LoxObject::Boolean { location, .. } => *location,
            LoxObject::Nil { location, .. } => *location,
        }
    }

    pub fn type_to_string(&self) -> String {
        match self {
            LoxObject::String { .. } => "string".to_string(),
            LoxObject::Number { .. } => "number".to_string(),
            LoxObject::Boolean { .. } => "boolean".to_string(),
            LoxObject::Nil { .. } => "nil".to_string(),
        }
    }
}

impl Display for LoxObject {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            LoxObject::String { value, .. } => writeln!(f, "{}", value),
            LoxObject::Number { value, .. } => writeln!(f, "{}", value),
            LoxObject::Boolean { value, .. } => writeln!(f, "{}", value),
            LoxObject::Nil { .. } => writeln!(f, "nil"),
        }
    }
}
