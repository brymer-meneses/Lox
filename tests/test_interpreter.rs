use lox::{ast::StmtVisitor, interpreter::Interpreter, parse_statements};

#[test]
fn interpret_addition() {
    let result = parse_statements(r"5.5 + 6.5;");
    let result = result.unwrap();

    let mut interpreter = Interpreter::new();
    let _result = interpreter.execute(&result[0]);
}
