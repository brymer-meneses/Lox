#[derive(Debug, PartialEq, Clone)]
pub enum LoxObject {
    String(String),
    Number(f64),
    Boolean(bool),
    Nil,
}
