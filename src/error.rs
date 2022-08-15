use crate::{object::LoxObject, source_location::SourceLocation};

use colored::*;

pub type LoxResult<T> = Result<T, LoxError>;

#[derive(Debug)]
pub enum LoxError {
    // scanner errors
    UnterminatedString {
        location: SourceLocation,
    },
    UnexpectedChar {
        location: SourceLocation,
        char: char,
    },
    // parser errors
    ExpectedStatement {
        location: SourceLocation,
    },
    ExpectedExpression {
        location: SourceLocation,
    },
    ExpectedToken {
        location: SourceLocation,
        message: String,
    },
    ExpectedSemicolon {
        location: SourceLocation,
    },
    InvalidVariableName {
        location: SourceLocation,
        variable: String,
    },
    InvalidFunctionName {
        location: SourceLocation,
        variable: String,
    },
    UnexpectedToken {
        location: SourceLocation,
        token: String,
    },
    InvalidAssignmentTarget {
        location: SourceLocation,
        lexeme: String,
    },
    // interpreter errors
    InvalidBinaryOperation {
        location: SourceLocation,
        left: String,
        operator: String,
        right: String,
    },
    InvalidUnaryOperation {
        location: SourceLocation,
        operator: String,
        right: String,
    },
    InvalidAssignment {
        location: SourceLocation,
        identifier: String,
    },
    UndefinedVariable {
        location: SourceLocation,
        variable: String,
    },
    ExceededMaximumArity {
        location: SourceLocation,
    },
    InvalidCall {
        location: SourceLocation,
    },

    Return {
        value: LoxObject,
    },
}

impl LoxError {
    pub fn raise(&self, source_code: &str) {
        let error: ColoredString = "error".red();

        match &self {
            LoxError::UnexpectedChar { char, location } => {
                eprintln!(
                    "{error}: Unexpected character: {}",
                    char.to_string().yellow()
                );
                highlight_location(source_code, location);
            }
            LoxError::UnterminatedString { location } => {
                eprintln!("{error}: Unterminated string here.");
                highlight_location(source_code, location);
            }
            LoxError::ExpectedToken { message, location } => {
                eprintln!("{error}: {}", message);
                highlight_location(source_code, location);
            }
            LoxError::ExpectedExpression { location } => {
                eprintln!("{error}: Expected expression here.");
                highlight_location(source_code, location);
            }
            LoxError::UnexpectedToken { token, location } => {
                eprintln!("{error}: Unexpected token `{token}`");
                highlight_location(source_code, location);
            }
            LoxError::InvalidAssignmentTarget { lexeme, location } => {
                eprintln!("{error}: Unexpected token `{lexeme}`");
                highlight_location(source_code, location);
            }
            LoxError::InvalidVariableName { variable, location } => {
                eprintln!("{error}: Got invalid variable name: `{}`", variable);
                highlight_location(source_code, location);
            }
            LoxError::InvalidBinaryOperation {
                location,
                left,
                operator,
                right,
            } => {
                eprintln!(
                    "{error}: The operation `{}` is invalid for `{}` and `{}",
                    operator, left, right
                );
                highlight_location(source_code, location);
            }
            LoxError::InvalidUnaryOperation {
                operator,
                right,
                location,
            } => {
                eprintln!(
                    "{error}: The operation `{}` is invalid for `{}`",
                    operator, right
                );
                highlight_location(source_code, location);
            }
            LoxError::InvalidAssignment {
                identifier,
                location,
            } => {
                eprintln!(
                    "{error}: Tried assigning value to `{}` which is undefined.",
                    identifier
                );
                highlight_location(source_code, location);
            }
            LoxError::UndefinedVariable { variable, location } => {
                eprintln!(
                    "{error}: Tried accessing the variable `{}` which is undefined.",
                    variable
                );
                highlight_location(source_code, location);
            }
            LoxError::ExpectedSemicolon { location } => {
                eprintln!("{error}: Expected semicolon here.");
                highlight_location(source_code, location);
            }
            _ => todo!(),
        };
    }
}

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

pub fn highlight_location(source_code: &str, location: &SourceLocation) {
    let line_num = location.line_start;
    let line_num_string_length = line_num.to_string().len();

    let source = source_code.lines().nth(line_num - 1).unwrap();

    let sep = "|".cyan();
    let offset = 2;
    let context = format!("  {}", line_num).cyan();
    let column = " ".repeat(offset + line_num_string_length);

    let arrows = format_arrows(location.start, location.end).yellow();

    eprintln!();
    eprintln!("{column} {sep}");
    eprintln!("{context} {sep} {source}");
    eprintln!("{column} {sep} {arrows}");
    eprintln!();
}
