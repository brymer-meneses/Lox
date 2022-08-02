use crate::source_location::SourceLocation;

use colored::*;

pub type LoxResult<T> = Result<T, LoxError>;

#[derive(Debug)]
pub struct LoxError {
    pub location: SourceLocation,
    pub kind: LoxErrorKind,
}

#[derive(Debug)]
pub enum LoxErrorKind {
    // scanner errors
    UnterminatedString,
    UnexpectedChar {
        char: char,
    },
    // parser errors
    ExpectedStatement,
    ExpectedExpression,
    ExpectedToken {
        token: String,
    },
    ExpectedVariableName {
        variable: String,
    },
    UnexpectedToken {
        token: String,
    },
    InvalidAssignmentTarget {
        lexeme: String,
    },
    // interpreter errors
    InvalidBinaryOperation {
        left: String,
        operator: String,
        right: String,
    },
    InvalidUnaryOperation {
        operator: String,
        right: String,
    },
    InvalidAssignment {
        identifier: String,
    },
    UndefinedVariable {
        variable: String,
    },
}

impl LoxError {
    pub fn new(kind: LoxErrorKind, location: SourceLocation) -> Self {
        LoxError { location, kind }
    }
    pub fn raise(&self, is_on_repl: bool, source_code: &str) {
        match &self.kind {
            LoxErrorKind::UnexpectedChar { char } => {
                eprintln!(
                    "{}: Unexpected character: {}",
                    "error".red(),
                    char.to_string().yellow()
                );
            }
            LoxErrorKind::UnterminatedString => {
                eprintln!("{}: Unterminated string here.", "error".red());
            }
            LoxErrorKind::ExpectedToken { token } => {
                eprintln!("{}: Expected token `{}` here.", "error".red(), token);
            }
            LoxErrorKind::ExpectedExpression => {
                eprintln!("{}: Expected expression here.", "error".red());
            }
            LoxErrorKind::UnexpectedToken { token } => {
                eprintln!("{}: Unexpected token `{}`", "error".red(), token);
            }
            LoxErrorKind::InvalidAssignmentTarget { lexeme } => {
                eprintln!("{}: Unexpected token `{}`", "error".red(), lexeme);
            }
            LoxErrorKind::ExpectedVariableName { variable } => {
                eprintln!(
                    "{}: Got invalid variable name: `{}`",
                    "error".red(),
                    variable
                );
            }
            LoxErrorKind::InvalidBinaryOperation {
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
            }
            LoxErrorKind::InvalidUnaryOperation { operator, right } => {
                eprintln!(
                    "{}: The operation `{}` is invalid for `{}`",
                    "error".red(),
                    operator,
                    right
                );
            }
            LoxErrorKind::InvalidAssignment { identifier } => {
                eprintln!(
                    "{}: Tried assigning value to `{}` which is undefined.",
                    "error".red(),
                    identifier
                );
            }
            LoxErrorKind::UndefinedVariable { variable } => {
                eprintln!(
                    "{}: Tried accessing the variable `{}` which is undefined.",
                    "error".red(),
                    variable
                );
            }
            _ => {
                todo!()
            }
        };
        highlight_location(is_on_repl, source_code, &self.location);
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
