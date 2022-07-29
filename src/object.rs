#[derive(Debug, PartialEq, Clone)]
pub enum LoxObject {
    String(String),
    Number(f64),
    Boolean(bool),
    Nil,
}

impl LoxObject {
    pub fn new(lexeme: &str) -> Self {
        if lexeme.eq("true") {
            return LoxObject::Boolean(true);
        }

        if lexeme.eq("false") {
            return LoxObject::Boolean(false);
        }

        let num = lexeme.parse::<f64>();
        if num.is_ok() {
            return LoxObject::Number(num.unwrap());
        }

        return LoxObject::String(lexeme.to_string());
    }
}
