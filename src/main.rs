fn main() {
    let args: Vec<String> = std::env::args().collect();

    match args.len() {
        1 => lox::run_prompt(),
        2 => lox::run_file(&args[1]),
        _ => eprintln!("ERROR: Expected usage: lox <filename>"),
    }
}
