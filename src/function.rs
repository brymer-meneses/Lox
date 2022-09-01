use crate::environment::Environment;
use crate::{
    ast::Stmt, error::LoxError, interpreter::Interpreter, object::LoxObject, token::Token,
};

use crate::error::LoxResult;

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
    pub fn call(
        &self,
        interpreter: &mut Interpreter,
        arguments: &[LoxObject],
    ) -> LoxResult<LoxObject> {
        match self {
            Function::Native { body, .. } => Ok(body(arguments.to_vec())),
            Function::User { name, params, body } => {
                let environment = Environment::wrap(&interpreter.globals);

                for (param, argument) in params.iter().zip(arguments.iter()) {
                    environment
                        .borrow_mut()
                        .define(param.lexeme.clone(), argument.clone());
                }

                match interpreter.execute_block(body, environment) {
                    Ok(..) => Ok(LoxObject::Nil {
                        location: name.location,
                    }),
                    Err(LoxError::Return { value }) => Ok(value),
                    Err(e) => Err(e),
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
