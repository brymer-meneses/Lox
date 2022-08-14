use crate::{ ast::Stmt, interpreter::Interpreter, object::LoxObject, token::Token, };

#[derive(Debug, Clone, PartialEq)]
pub enum Function {
    Native {
        arity: usize,
        body: fn(Vec<LoxObject>) -> LoxObject,
    },

    User {
        name: Box<Token>,
        params: Vec<Token>,
        body: Vec<Stmt>,
    },
}

impl Function {
    pub fn call(&self, interpreter: &mut Interpreter, arguments: &[LoxObject]) -> LoxObject {
        match self {
            Function::Native { body, .. } => body(arguments.to_vec()),
            Function::User { name, params, body } => {
                let environment = interpreter.globals.clone();

                for (param, argument) in params.iter().zip(arguments.iter()) {
                    environment
                        .borrow_mut()
                        .define(param.lexeme.clone(), argument.clone());
                }

                match interpreter.execute_block(body, environment) {
                    Ok(..) => LoxObject::Nil {
                        location: name.location,
                    },
                    _ => panic!("Error!"),
                }
            }
        }
    }
    pub fn arity(&self) -> usize {
        match self {
            Function::Native { arity, .. } => *arity,
            Function::User { params, .. } => params.len(),
        }
    }
}
