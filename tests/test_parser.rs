use lox::object::LoxObject;
use lox::parser::Parser;
use lox::source_location::SourceLocation;
use lox::syntax::{Stmt, Expr};
use lox::token::TokenType;
use lox::token::Token;


#[test]
fn it_should_parse_primary() {
    let dummy = SourceLocation::new_single_line(1, 0, 0);
    let tokens = vec![
        Token::new(TokenType::Number, dummy.clone(), "5.55".to_string(), Some(LoxObject::Number(5.55))),
        Token::new(TokenType::EOF, dummy.clone(), "".to_string(), None),
    ];
    let mut parser = Parser::new(&tokens);
    let statements = parser.parse().unwrap();

    let expr = match &statements[0] {
        Stmt::Expression(expr) => expr,
        _ => panic!("Wrong"),
    };

    let expected = Box::new(Expr::Literal(LoxObject::Number(5.55)));
    assert_eq!(expr, &expected);
}

#[test]
fn it_should_parse_grouping() {
    let dummy = SourceLocation::new_single_line(1, 0, 0);
    let tokens = vec![
        Token::new(TokenType::LeftParen, dummy.clone(), "(".to_string(), None),
        Token::new(TokenType::String, dummy.clone(), "".to_string(), Some(LoxObject::String("The quick brown fox jumped over the lazy cat.".to_string()))),
        Token::new(TokenType::RightParen, dummy.clone(), ")".to_string(), None),
        Token::new(TokenType::EOF, dummy.clone(), "".to_string(), None),
    ];
    let mut parser = Parser::new(&tokens);
    let statements = parser.parse().unwrap();

    let expr = match &statements[0] {
        Stmt::Expression(expr) => expr,
        _ => panic!("Wrong"),
    };

    let expected = Box::new(Expr::Grouping(
        Box::new(
            Expr::Literal(LoxObject::String("The quick brown fox jumped over the lazy cat.".to_string())))
       )
    );
    assert_eq!(expr, &expected);
}
