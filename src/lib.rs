pub mod ast;
pub mod environment;
pub mod error;
pub mod function;
pub mod interpreter;
pub mod object;
pub mod parser;
pub mod scanner;
pub mod source_location;
pub mod token;

use colored::{Color, Colorize};

use crate::error::LoxError;
use crate::interpreter::Interpreter;
use crate::parser::Parser;
use crate::scanner::Scanner;

use crate::ast::Stmt;

use rustyline::Editor;

pub fn run_prompt() {
    let mut interpreter = Interpreter::new();
    let mut rl = Editor::<()>::new().expect("Something went wrong starting prompt.");

    while let Some(mut line) = readline(&mut rl, ">>> ", Color::BrightBlue) {
        if line.trim().is_empty() {
            break;
        };

        while let Err(LoxError::ExpectedToken { .. }) = parse_statements(&line) {
            let extension_line = match readline(&mut rl, "... ", Color::Black) {
                Some(line) => line,
                None => break,
            };

            if extension_line.trim().is_empty() {
                break;
            };
            line.push_str(&extension_line);
            continue;
        }

        run(&line, &mut interpreter);
    }
}

pub fn run_file(filename: &str) {
    let mut interpreter = Interpreter::new();
    let contents = std::fs::read_to_string(filename).unwrap_or_else(|err| {
        eprintln!("ERROR: {}", err);
        std::process::exit(1);
    });

    run(&contents, &mut interpreter);
}

fn run(source_code: &str, interpreter: &mut Interpreter) {
    match parse_statements(source_code) {
        Ok(statements) => {
            if let Err(interpreter_error) = interpreter.interpret(&statements) {
                interpreter_error.raise(source_code);
            }
        }
        Err(error) => {
            error.raise(source_code);
        }
    }
}

fn parse_statements(source_code: &str) -> Result<Vec<Stmt>, LoxError> {
    let mut scanner = Scanner::new(source_code);
    let tokens = scanner.scan()?;

    let mut parser = Parser::new(tokens);
    let statements = parser.parse()?;

    Ok(statements)
}

fn readline(rl: &mut Editor<()>, prompt: &str, color: Color) -> Option<String> {
    let prompt = prompt.color(color).to_string();

    let result = rl.readline(&prompt);
    match result {
        Ok(mut line) => {
            rl.add_history_entry(line.as_str());
            line.push('\n');
            Some(line)
        }
        _ => None,
    }
}
