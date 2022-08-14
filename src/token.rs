use std::default;

use crate::object::LoxObject;
use crate::source_location::SourceLocation;

#[derive(Debug, Clone, PartialEq)]
pub struct Token {
    pub kind: TokenType,
    pub location: SourceLocation,
    pub lexeme: String,
    pub literal: Option<LoxObject>,
}

#[derive(PartialEq, Eq, Debug, Clone, Default)]
pub enum TokenType {
    // Single character tokens
    LeftBrace,
    RightBrace,
    LeftParen,
    RightParen,

    // One or two character tokens
    Comma,
    Minus,
    Plus,
    Semicolon,
    Slash,
    Star,
    Dot,

    Bang,
    BangEqual,
    Equal,
    EqualEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,

    // Literals
    Identifier,
    String,
    Number,

    // Keywords
    Class,
    Function,
    Else,
    False,
    For,
    If,
    Nil,
    Or,
    And,
    Print,
    Return,
    Super,
    This,
    True,
    Var,
    While,

    #[default]
    EOF,
}

impl Token {
    pub fn new(
        kind: TokenType,
        location: SourceLocation,
        lexeme: String,
        literal: Option<LoxObject>,
    ) -> Self {
        Token {
            kind,
            location,
            lexeme,
            literal,
        }
    }

    pub fn keyword(keyword: &str) -> TokenType {
        match keyword {
            "and" => TokenType::And,
            "class" => TokenType::Class,
            "else" => TokenType::Else,
            "false" => TokenType::False,
            "for" => TokenType::For,
            "fun" => TokenType::Function,
            "if" => TokenType::If,
            "nil" => TokenType::Nil,
            "or" => TokenType::Or,
            "print" => TokenType::Print,
            "return" => TokenType::Return,
            "super" => TokenType::Super,
            "this" => TokenType::This,
            "true" => TokenType::True,
            "var" => TokenType::Var,
            "while" => TokenType::While,
            _ => TokenType::Identifier,
        }
    }
}
