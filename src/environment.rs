use std::collections::HashMap;

use crate::object::LoxObject;
use std::rc::Rc;
use std::cell::RefCell;

#[derive(Clone, Debug, PartialEq)]
pub struct Environment {
    enclosing: Option<Rc<RefCell<Environment>>>,
    values: HashMap<String, LoxObject>,
}

impl Environment {
    pub fn new(enclosing: Option<&Rc<RefCell<Environment>>>) -> Self {
        Environment {
            enclosing: enclosing.map(Rc::clone),
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
            enclosing.borrow_mut().assign(lexeme, object);
        }
    }

    pub fn retrieve(&self, lexeme: &str) -> Option<LoxObject> {
        if let Some(value) = self.values.get(lexeme) {
            return Some(value.clone());
        };

        if let Some(enclosing) = &self.enclosing {
            return enclosing.borrow().retrieve(lexeme);
        }

        None
    }

    pub fn length(&self) -> usize {
        self.values.len()
    }

    pub fn wrap(enclosing: Rc<RefCell<Environment>>) -> Rc<RefCell<Environment>> {
        let environment = Environment {
            enclosing: Some(enclosing),
            values: HashMap::new(),
        };
        Rc::new(RefCell::new(environment))
    }
}
