use crate::source_location::SourceLocation;

use colored::*;

pub trait LoxError {
    fn report(&self, is_on_repl: bool, source_code: &str);
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
    UnexpectedToken(SourceLocation, String),
}

impl LoxError for ScannerError {
    fn report(&self, is_on_repl: bool, source_code: &str) {
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
        highlight_location(is_on_repl, source_code, &location)
    }
}

impl LoxError for ParserError {
    fn report(&self, is_on_repl: bool, source_code: &str) {
        match self {
            ParserError::ExpectedToken(location, kind) => {
                eprintln!("{}: Expected token {:?}", "error".red(), kind);
                highlight_location(is_on_repl, source_code, &location)
            }
            ParserError::UnexpectedToken(location, lexeme) => {
                eprintln!("{}: Unexpected token {:?}", "error".red(), lexeme);
                highlight_location(is_on_repl, source_code, &location)
            }
            _ => {}
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
