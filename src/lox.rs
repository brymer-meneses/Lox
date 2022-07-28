use std::io::Write;

use crate::{scanner::Scanner, source_location::SourceLocation};
use crate::scanner::ScannerError;

pub struct Lox {
    scanner: Scanner,
}

pub trait LoxError {
    fn report(&self);

}


impl Lox {

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

            Self::run(true, &line);
        }
    }

    pub fn run_file(filename: &str) {
        let contents = std::fs::read_to_string(filename).unwrap_or_else(|err| {
            eprintln!("ERROR: {}", err);
            std::process::exit(1);
        });

        Self::run(false, &contents);
    }


    pub fn run(is_on_repl: bool, source_code: &str) {
        let mut scanner = Scanner::new(source_code);
        let tokens = scanner.scan();
        let tokens = match tokens {
            Ok(tok) => tok,
            Err(err) => {
                Lox::report(is_on_repl, source_code, err);
                return;
            },
        };

        for token in tokens.iter() {
            println!("{:#?}", token);
        }
    }

    fn highlight_location(is_on_repl: bool, source_code: &str, location: &SourceLocation) {
        let offset = 5;

        // let is_multiline = location.line_start != location.line_end;
        // if is_multiline { return };

        let line_num = location.line_start;
        let prompt = if is_on_repl { ">".to_string() } else { line_num.to_string() };

        let prompt_code = format!("{} | {}", prompt, source_code.lines().nth(line_num -1).unwrap());

        eprintln!("\n{}{}\n", " ".repeat(offset), prompt_code);
    }

    fn report(is_on_repl: bool, source_code: &str, error: ScannerError) {
        match error {
            ScannerError::UnexpectedChar(pos, c) => {
                Lox::highlight_location(is_on_repl, source_code, &pos);
                eprintln!("ERROR: Unexpected character {}", c);
            },
            ScannerError::UnterminatedString(pos) => {
                Lox::highlight_location(is_on_repl, source_code, &pos);
                eprintln!("ERROR: Unterminated string");
            },
        }
    }

}

