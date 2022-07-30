use std::io::Write;

use lox::error::LoxError;
use lox::interpreter::Interpreter;
use lox::parser::Parser;
use lox::scanner::Scanner;

fn main() {
    let args: Vec<String> = std::env::args().collect();

    match args.len() {
        1 => run_prompt(),
        2 => run_file(&args[1]),
        _ => eprintln!("ERROR: Expected usage: lox <filename>"),
    }
}

pub fn run_prompt() {
    let mut interpreter = Interpreter::new(None);
    loop {
        let mut line = String::new();

        print!(">>> ");
        let _ = std::io::stdout().flush();
        std::io::stdin()
            .read_line(&mut line)
            .expect("ERROR: did not enter a correct string");

        if line.trim().is_empty() {
            break;
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

pub fn run(is_on_repl: bool, source_code: &str, interpreter: &mut Interpreter) {
    let mut scanner = Scanner::new(source_code);
    let tokens = match scanner.scan() {
        Ok(tok) => tok,
        Err(scanner_error) => {
            scanner_error.raise(is_on_repl, source_code);
            return;
        }
    };

    let mut parser = Parser::new(tokens);
    let statements = match parser.parse() {
        Ok(value) => value,
        Err(parser_error) => {
            parser_error.raise(is_on_repl, source_code);
            return;
        }
    };

    if let Err(interpreter_error) = interpreter.interpret(statements) {
        interpreter_error.raise(is_on_repl, source_code);
    }
}
