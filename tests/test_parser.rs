use lox::object::LoxObject;
use lox::parser::Parser;
use lox::source_location::SourceLocation;
use lox::syntax::{Expr, Stmt};
use lox::token::Token;
use lox::token::TokenType;

#[test]
fn it_should_parse_primary() {
    let location = SourceLocation::new_single_line(1, 0, 0);
    let tokens = vec![
        Token::new(
            TokenType::Number,
            location,
            "5.55".to_string(),
            Some(LoxObject::Number(5.55)),
        ),
        Token::new(TokenType::EOF, location, "".to_string(), None),
    ];
    let mut parser = Parser::new(&tokens);
    let statements = parser.parse().unwrap();

    let expr = match &statements[0] {
        Stmt::Expression { expression, .. } => expression,
        _ => panic!("Wrong"),
    };

    let expected = Expr::Literal {
        location,
        literal: LoxObject::Number(5.55),
    };
    assert_eq!(expr, &expected);
}

#[test]
fn it_should_parse_grouping() {
    let location = SourceLocation::new_single_line(1, 0, 0);
    let literal = LoxObject::String("The quick brown fox jumped over the lazy cat.".to_string());
    let tokens = vec![
        Token::new(TokenType::LeftParen, location, "(".to_string(), None),
        Token::new(
            TokenType::String,
            location,
            "".to_string(),
            Some(literal.clone()),
        ),
        Token::new(TokenType::RightParen, location, ")".to_string(), None),
        Token::new(TokenType::EOF, location, "".to_string(), None),
    ];
    let mut parser = Parser::new(&tokens);
    let statements = parser.parse().unwrap();

    let expr = match &statements[0] {
        Stmt::Expression { expression, .. } => expression,
        _ => panic!("Wrong"),
    };

    let expected = Expr::Grouping {
        location,
        expression: Box::new(Expr::Literal { location, literal }),
    };
    assert_eq!(expr, &expected);
}
