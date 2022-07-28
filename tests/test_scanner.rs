use lox::object::LoxObject;
use lox::scanner::Scanner;
use lox::token::TokenType;

#[test]
fn it_should_scan_single_char_tokens() {
    let mut scanner = Scanner::new("{ } ( ) / . ! + - * > < =");
    let tokens = scanner.scan().unwrap();

    let correct_types = vec![
        TokenType::LeftBrace,
        TokenType::RightBrace,
        TokenType::LeftParen,
        TokenType::RightParen,
        TokenType::Slash,
        TokenType::Dot,
        TokenType::Bang,
        TokenType::Plus,
        TokenType::Minus,
        TokenType::Star,
        TokenType::Greater,
        TokenType::Less,
        TokenType::Equal,
        TokenType::EOF,
    ];

    for (i, token) in tokens.iter().enumerate() {
        assert_eq!(token.token_type, correct_types[i]);
    }
}

#[test]
fn it_should_scan_double_char_tokens() {
    let mut scanner = Scanner::new("== != >= <=");
    let tokens = scanner.scan().unwrap();

    let correct_types = vec![
        TokenType::EqualEqual,
        TokenType::BangEqual,
        TokenType::GreaterEqual,
        TokenType::LessEqual,
        TokenType::EOF,
    ];

    for (i, token) in tokens.iter().enumerate() {
        assert_eq!(token.token_type, correct_types[i]);
    }
}

#[test]
fn it_should_recognize_comments() {
    let mut scanner = Scanner::new("// hi there");
    let tokens = scanner.scan().unwrap();

    assert_eq!(tokens[0].token_type, TokenType::EOF);
}

#[test]
fn it_should_scan_string() {
    let mut scanner = Scanner::new(" \"The quick brown fox jumped over the lazy cat.\" \"If you're reading this have a good day!\" ");
    let tokens = scanner.scan().unwrap();

    assert_eq!(tokens[0].token_type, TokenType::String);
    assert_eq!(tokens[0].literal, Some(LoxObject::String("The quick brown fox jumped over the lazy cat.".to_string())));
    assert_eq!(tokens[0].lexeme, "\"The quick brown fox jumped over the lazy cat.\"");

    assert_eq!(tokens[1].token_type, TokenType::String);
    assert_eq!(tokens[1].literal, Some(LoxObject::String("If you're reading this have a good day!".to_string())));
    assert_eq!(tokens[1].lexeme, "\"If you're reading this have a good day!\"");
}

#[test]
fn it_should_scan_keywords() {
    let mut scanner = Scanner::new("and class else false for fun if nil or print return super this true var while keyword");
    let tokens = scanner.scan().unwrap();

    let correct_types = vec![
        TokenType::And,
        TokenType::Class,
        TokenType::Else,
        TokenType::False,
        TokenType::For,
        TokenType::Function,
        TokenType::If,
        TokenType::Nil,
        TokenType::Or,
        TokenType::Print,
        TokenType::Return,
        TokenType::Super,
        TokenType::This,
        TokenType::True,
        TokenType::Var,
        TokenType::While,
        TokenType::Identifier,
        TokenType::EOF,
    ];

    for (i, token) in tokens.iter().enumerate() {
        assert_eq!(token.token_type, correct_types[i]);
    };
}

#[test]
fn it_should_scan_numbers() {
    let mut scanner = Scanner::new("3.1415926536 2.7182818284");
    let tokens = scanner.scan().unwrap();

    assert_eq!(tokens[0].literal, Some(LoxObject::Number(3.1415926536)));
    assert_eq!(tokens[1].literal, Some(LoxObject::Number(2.7182818284)));
}
