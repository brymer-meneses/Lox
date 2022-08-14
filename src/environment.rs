use std::collections::{hash_map::Entry, HashMap};

use crate::object::LoxObject;
use std::cell::RefCell;
use std::rc::Rc;

#[derive(Clone, Debug)]
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

    pub fn assign(&mut self, lexeme: String, object: LoxObject) -> Result<(), ()> {
        if let Entry::Occupied(mut e) = self.values.entry(lexeme.to_owned()) {
            e.insert(object);
            return Ok(());
        }

        if let Some(enclosing) = &mut self.enclosing {
            enclosing.borrow_mut().assign(lexeme, object)?;
            return Ok(());
        }

        Err(())
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
