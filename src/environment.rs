use std::collections::HashMap;

use crate::object::LoxObject;

#[derive(Clone)]
pub struct Environment {
    enclosing: Option<Box<Environment>>,
    values: HashMap<String, LoxObject>,
}

impl Environment {
    pub fn new(enclosing: Option<Environment>) -> Self {
        let enclosing = match enclosing {
            Some(value) => Some(Box::new(value)),
            None => None,
        };
        Environment {
            enclosing,
            values: HashMap::new(),
        }
    }

    pub fn define(&mut self, lexeme: String, object: LoxObject) {
        self.values.insert(lexeme, object);
    }

    pub fn assign(&mut self, lexeme: String, object: LoxObject) {
        if let std::collections::hash_map::Entry::Occupied(mut e) =
            self.values.entry(lexeme.clone())
        {
            e.insert(object);
            return;
        }

        if let Some(enclosing) = &mut self.enclosing {
            enclosing.assign(lexeme, object);
        }
    }

    pub fn retrieve(&self, lexeme: &str) -> Option<LoxObject> {
        if let Some(value) = self.values.get(lexeme) {
            return Some(value.clone());
        };

        if let Some(enclosing) = &self.enclosing {
            return enclosing.retrieve(lexeme);
        }

        None
    }

    pub fn length(&self) -> usize {
        self.values.len()
    }
}
