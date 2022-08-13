use crate::source_location::SourceLocation;

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
        token: String,
    },
    ExpectedVariableName {
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
}

impl LoxError {
    pub fn raise(&self, is_on_repl: bool, source_code: &str) {
        match &self {
            LoxError::UnexpectedChar { char, location } => {
                eprintln!(
                    "{}: Unexpected character: {}",
                    "error".red(),
                    char.to_string().yellow()
                );
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::UnterminatedString { location } => {
                eprintln!("{}: Unterminated string here.", "error".red());
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::ExpectedToken { token, location } => {
                eprintln!("{}: Expected token `{}` here.", "error".red(), token);
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::ExpectedExpression { location } => {
                eprintln!("{}: Expected expression here.", "error".red());
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::UnexpectedToken { token, location } => {
                eprintln!("{}: Unexpected token `{}`", "error".red(), token);
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::InvalidAssignmentTarget { lexeme, location } => {
                eprintln!("{}: Unexpected token `{}`", "error".red(), lexeme);
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::ExpectedVariableName { variable, location } => {
                eprintln!(
                    "{}: Got invalid variable name: `{}`",
                    "error".red(),
                    variable
                );
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::InvalidBinaryOperation {
                location,
                left,
                operator,
                right,
            } => {
                eprintln!(
                    "{}: The operation `{}` is invalid for `{}` and `{}",
                    "error".red(),
                    operator,
                    left,
                    right
                );
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::InvalidUnaryOperation {
                operator,
                right,
                location,
            } => {
                eprintln!(
                    "{}: The operation `{}` is invalid for `{}`",
                    "error".red(),
                    operator,
                    right
                );
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::InvalidAssignment {
                identifier,
                location,
            } => {
                eprintln!(
                    "{}: Tried assigning value to `{}` which is undefined.",
                    "error".red(),
                    identifier
                );
                highlight_location(is_on_repl, source_code, location);
            }
            LoxError::UndefinedVariable { variable, location } => {
                eprintln!(
                    "{}: Tried accessing the variable `{}` which is undefined.",
                    "error".red(),
                    variable
                );
                highlight_location(is_on_repl, source_code, location);
            }
            _ => {
                todo!()
            }
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

pub fn highlight_location(is_on_repl: bool, source_code: &str, location: &SourceLocation) {
    let line_num = location.line_start;
    let line_num_string_length = line_num.to_string().len();

    let source = source_code.lines().nth(line_num - 1).unwrap();

    let sep = "|".cyan();
    let repl_col_offset = 3;
    let file_col_offset = 2;
    let repl_context = "  >".cyan();
    let file_context = format!("  {}", line_num).cyan();

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

    eprintln!();
    eprintln!("{column} {sep}");
    eprintln!("{context} {sep} {source}");
    eprintln!("{column} {sep} {arrows}");
    eprintln!();
}
