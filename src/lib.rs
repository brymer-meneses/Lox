pub mod ast;
pub mod environment;
pub mod error;
pub mod interpreter;
pub mod object;
pub mod parser;
pub mod scanner;
pub mod source_location;
pub mod token;

use std::io::Write;

use colored::{Color, Colorize};
use error::{ParserError, ScannerError};

use crate::error::LoxError;
use crate::interpreter::Interpreter;
use crate::parser::Parser;
use crate::scanner::Scanner;

use crate::ast::Stmt;

pub fn run_prompt() {
    let mut interpreter = Interpreter::new(None);
    loop {
        let mut line = line_input(">>> ", Color::BrightBlue);
        if line.trim().is_empty() {
            break;
        };

        while let Err(Error::ParserError(parser_error)) = check_syntax(&line) {
            if let ParserError::ExpectedToken(..) = parser_error {
                let extension_line = line_input("... ", Color::Black);
                if extension_line.trim().is_empty() { break; };
                line.push_str(&extension_line);
            }
        }

        run(true, &line, &mut interpreter);
    }
}

pub fn run_file(filename: &str) {
    let mut interpreter = Interpreter::new(None);
    let contents = std::fs::read_to_string(filename).unwrap_or_else(|err| {
        eprintln!("ERROR: {}", err);
        std::process::exit(1);
    });

    run(false, &contents, &mut interpreter);
}

fn run(is_on_repl: bool, source_code: &str, interpreter: &mut Interpreter) {
    match check_syntax(source_code) {
        Ok(statements) => {
            if let Err(interpreter_error) = interpreter.interpret(&statements) {
                interpreter_error.raise(is_on_repl, source_code);
            }
        }
        Err(error) => match error {
            Error::ParserError(parser_error) => {
                parser_error.raise(is_on_repl, source_code);
            }
            Error::ScannerError(scanner_error) => {
                scanner_error.raise(is_on_repl, source_code);
            }
        },
    }
}

enum Error {
    ParserError(ParserError),
    ScannerError(ScannerError),
}

fn check_syntax(source_code: &str) -> Result<Vec<Stmt>, Error> {
    let mut scanner = Scanner::new(source_code);
    let tokens = match scanner.scan() {
        Ok(value) => value,
        Err(scanner_error) => {
            return Err(Error::ScannerError(scanner_error));
        }
    };

    let mut parser = Parser::new(tokens);
    let statements = match parser.parse() {
        Ok(value) => value,
        Err(parser_error) => {
            return Err(Error::ParserError(parser_error));
        }
    };

    return Ok(statements.to_vec());
}

fn line_input(prompt: &str, color: Color) -> String {
    let mut line = String::new();
    print!("{}", prompt.color(color));

    let _ = std::io::stdout().flush();
    std::io::stdin()
        .read_line(&mut line)
        .expect("ERROR: did not enter a correct string");

    line
}
