use std::io::Write;

use crate::error::LoxError;
use crate::parser::Parser;
use crate::scanner::Scanner;

pub fn run_prompt() {
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

        run(true, &line);
    }
}

pub fn run_file(filename: &str) {
    let contents = std::fs::read_to_string(filename).unwrap_or_else(|err| {
        eprintln!("ERROR: {}", err);
        std::process::exit(1);
    });

    run(false, &contents);
}

pub fn run(is_on_repl: bool, source_code: &str) {
    let mut scanner = Scanner::new(source_code);
    let tokens = match scanner.scan() {
        Ok(tok) => tok,
        Err(err) => {
            err.report(is_on_repl, source_code);
            return;
        }
    };

    let mut parser = Parser::new(tokens);
    let statements = match parser.parse() {
        Ok(value) => value,
        Err(err) => {
            err.report(is_on_repl, source_code);
            return;
        }
    };

    for statement in statements.iter() {
        println!("{:#?}", statement);
    }
}
