
use crate::object::LoxObject;
use crate::source_location::SourceLocation;

#[derive(PartialEq, Debug)]
pub struct Token {
    pub token_type: TokenType,
    pub source_location: SourceLocation,
    pub lexeme: String,
    pub literal: Option<LoxObject>,
}

#[derive(PartialEq, Debug)]
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

    // One or two character tokens
    EOF,
}

impl Token {
    pub fn new(
        token_type: TokenType,
        source_location: SourceLocation,
        lexeme: String,
        literal: Option<LoxObject>,
    ) -> Self {
        return Token {
            token_type,
            source_location,
            lexeme,
            literal,
        };
    }

    pub fn keyword(keyword: &str) -> TokenType {
        match keyword {
            "and"       => return TokenType::And,
            "class"     => return TokenType::Class,
            "else"      => return TokenType::Else,
            "false"     => return TokenType::False,
            "for"       => return TokenType::For,
            "fun"       => return TokenType::Function,
            "if"        => return TokenType::If,
            "nil"       => return TokenType::Nil,
            "or"        => return TokenType::Or,
            "print"     => return TokenType::Print,
            "return"    => return TokenType::Return,
            "super"     => return TokenType::Super,
            "this"      => return TokenType::This,
            "true"      => return TokenType::True,
            "var"       => return TokenType::Var,
            "while"     => return TokenType::While,
            _           => return TokenType::Identifier,
        }
    }
}
