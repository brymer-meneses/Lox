
use lox::lox::Lox;

fn main() {
    let args: Vec<String> = std::env::args().collect();

    match args.len() {
        1 => Lox::run_prompt(),
        2 => Lox::run_file(&args[1]),
        _ => eprintln!("ERROR: Expected usage: lox <filename>"),
    }
}

