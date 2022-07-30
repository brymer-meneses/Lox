use crate::source_location::SourceLocation;

use colored::*;

pub trait LoxError {
    fn raise(&self, is_on_repl: bool, source_code: &str);
}

#[derive(PartialEq, Debug)]
pub enum ScannerError {
    UnterminatedString(SourceLocation),
    UnexpectedChar(SourceLocation, char),
}

#[derive(PartialEq, Debug)]
pub enum ParserError {
    ExpectedToken(SourceLocation, String),
    ExpectedStatement(SourceLocation),
    ExpectedExpression(SourceLocation, String),
    ExpectedVariableName(SourceLocation),
    UnexpectedToken(SourceLocation, String),
}

#[derive(PartialEq, Debug)]
pub enum InterpreterError {
    InvalidBinaryOperation(SourceLocation, String, String, String),
    InvalidUnaryOperation(SourceLocation, String, String),
    InvalidAssignment(SourceLocation, String),
    UndefinedVariable(SourceLocation, String),
}

impl LoxError for ScannerError {
    fn raise(&self, is_on_repl: bool, source_code: &str) {
        let location = match self {
            ScannerError::UnexpectedChar(location, c) => {
                eprintln!(
                    "{}: Unexpected character: {}",
                    "error".red(),
                    c.to_string().yellow()
                );
                location
            }
            ScannerError::UnterminatedString(location) => {
                eprintln!("{}: Unterminated string here.", "error".red());
                location
            }
        };
        highlight_location(is_on_repl, source_code, location)
    }
}

impl LoxError for ParserError {
    fn raise(&self, is_on_repl: bool, source_code: &str) {
        match self {
            ParserError::ExpectedToken(location, kind) => {
                eprintln!("{}: Expected token {:?}", "error".red(), kind);
                highlight_location(is_on_repl, source_code, location)
            }
            ParserError::UnexpectedToken(location, lexeme) => {
                eprintln!("{}: Unexpected token {:?}", "error".red(), lexeme);
                highlight_location(is_on_repl, source_code, location)
            }
            _ => {
                todo!()
            }
        }
    }
}

impl LoxError for InterpreterError {
    fn raise(&self, is_on_repl: bool, source_code: &str) {
        match self {
            InterpreterError::InvalidBinaryOperation(location, left, operator, right) => {
                eprintln!(
                    "{}: The operation {} is invalid for {} and {}",
                    "error".red(),
                    operator,
                    left,
                    right
                );
                highlight_location(is_on_repl, source_code, location)
            }
            InterpreterError::InvalidUnaryOperation(location, operator, right) => {
                eprintln!(
                    "{}: The operation {} is invalid for {}",
                    "error".red(),
                    operator,
                    right
                );
                highlight_location(is_on_repl, source_code, location)
            }
            InterpreterError::InvalidAssignment(location, variable) => {
                eprintln!(
                    "{}: Tried assigning value to {} which is undefined.",
                    "error".red(),
                    variable
                );
                highlight_location(is_on_repl, source_code, location)
            }
            InterpreterError::UndefinedVariable(location, variable) => {
                eprintln!(
                    "{}: Tried accessing the variable `{}` which is undefined.",
                    "error".red(),
                    variable
                );
                highlight_location(is_on_repl, source_code, location)
            }
            _ => {
                todo!()
            }
        }
    }
}

pub fn highlight_location(is_on_repl: bool, source_code: &str, location: &SourceLocation) {
    let offset = 1;

    // let is_multiline = location.line_start != location.line_end;
    // if is_multiline { return };

    let line_num = location.line_start;
    let prompt = if is_on_repl {
        ">".to_string().cyan()
    } else {
        line_num.to_string().cyan()
    };
    let prompt = format!(
        "{} {} {}",
        prompt,
        "|".cyan(),
        source_code.lines().nth(line_num - 1).unwrap()
    );

    eprintln!("\n{}{}", " ".repeat(offset), prompt);
    eprintln!(
        "{}{}{}",
        " ".repeat(offset + 4),
        " ".repeat(location.start),
        "^".repeat(location.end - location.start + 1).yellow()
    );
}
