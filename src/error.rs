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
    ExpectedExpression(SourceLocation),
    ExpectedVariableName(SourceLocation, String),
    UnexpectedToken(SourceLocation, String),
    InvalidAssignmentTarget(SourceLocation, String),
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
                eprintln!("{}: Expected token `{}` here.", "error".red(), kind);
                highlight_location(is_on_repl, source_code, location)
            }
            ParserError::ExpectedExpression(location) => {
                eprintln!("{}: Expected expression here.", "error".red());
                highlight_location(is_on_repl, source_code, location)
            }
            ParserError::UnexpectedToken(location, lexeme) => {
                eprintln!("{}: Unexpected token `{}`", "error".red(), lexeme);
                highlight_location(is_on_repl, source_code, location)
            }
            ParserError::InvalidAssignmentTarget(location, lexeme) => {
                eprintln!("{}: Unexpected token `{}`", "error".red(), lexeme);
                highlight_location(is_on_repl, source_code, location)
            }
            ParserError::ExpectedVariableName(location, lexeme) => {
                eprintln!("{}: Got invalid variable name: `{}`", "error".red(), lexeme);
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
                    "{}: The operation `{}` is invalid for `{}` and `{}",
                    "error".red(),
                    operator,
                    left,
                    right
                );
                highlight_location(is_on_repl, source_code, location)
            }
            InterpreterError::InvalidUnaryOperation(location, operator, right) => {
                eprintln!(
                    "{}: The operation `{}` is invalid for `{}`",
                    "error".red(),
                    operator,
                    right
                );
                highlight_location(is_on_repl, source_code, location)
            }
            InterpreterError::InvalidAssignment(location, variable) => {
                eprintln!(
                    "{}: Tried assigning value to `{}` which is undefined.",
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

// struct ErrorMessage<'a> {
//     is_on_repl: bool,
//     source: &'a str,
//     location;''
//
// }
fn format_arrows(start: usize, end: usize) -> String {
    let mut retval = String::new();

    for i in 0..end + 1 {
        if i < start {
            retval.push(' ');
            continue;
        }
        retval.push('^');
    }
    retval
}

pub fn highlight_location(is_on_repl: bool, source_code: &str, location: &SourceLocation) {
    let line_num = location.line_start;
    let line_num_string_length = line_num.to_string().len();

    let source = source_code.lines().nth(line_num - 1).unwrap();

    let sep = "|".cyan();
    let repl_col_offset = 3;
    let file_col_offset = 2;
    let repl_context = "  >".cyan();
    let file_context = format!("  {}", line_num.to_string()).cyan();

    let column = if is_on_repl {
        " ".repeat(repl_col_offset)
    } else {
        " ".repeat(file_col_offset + line_num_string_length)
    };
    let context = if is_on_repl {
        repl_context
    } else {
        file_context
    };
    let arrows = format_arrows(location.start, location.end).yellow();

    eprint!("\n");
    eprintln!("{column} {sep}");
    eprintln!("{context} {sep} {source}");
    eprintln!("{column} {sep} {arrows}");
    eprint!("\n");
}
