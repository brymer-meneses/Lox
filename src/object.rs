use crate::{source_location::SourceLocation, error::{InterpreterError, LoxError}};
use std::{ops, fmt::Display};

#[derive(Debug, PartialEq, Clone)]
pub enum LoxObject {
    String {
        location: SourceLocation,
        value: String
    } ,
    Number {
        location: SourceLocation,
        value: f64
    },
    Boolean {
        location: SourceLocation,
        value: bool,
    },
    Nil {
        location: SourceLocation
    }
}

pub enum LoxType {
    String,
    Number,
    Boolean,
    Nil,
}


impl LoxObject {
    pub fn location(&self) -> SourceLocation {
        match &self {
            LoxObject::String{location, ..} => *location,
            LoxObject::Number{location, ..} => *location,
            LoxObject::Boolean{location, ..} => *location,
            LoxObject::Nil{location, ..} => *location,
        }
    }

    // fn hash(&self) -> LoxType {
    //     match self {
    //         LoxObject::String{..} => LoxType::String,
    //         LoxObject::Number{..} => 1,
    //         LoxObject::Boolean{..} => 2,
    //         LoxObject::Nil{..} => 3,
    //     }
    //
    // }

    // fn value(&self) ->

}

impl ops::Add<LoxObject> for LoxObject {
    type Output = Result<LoxObject, InterpreterError>;

    fn add(self, rhs: LoxObject) -> Result<LoxObject, InterpreterError> { 

        let location = self.location() + rhs.location();

        if let LoxObject::Number { value: val1, .. } = self { 
            if let LoxObject::Number { value: val2, .. } = rhs { 
                return Ok(LoxObject::Number { location, value: val1 + val2 })
            };
        };


        Err(InterpreterError::InvalidBinaryOperation(location, "+".to_string()))
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

