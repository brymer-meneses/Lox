use lox::environment::Environment;
use lox::object::LoxObject;
use lox::source_location::SourceLocation;

#[test]
fn it_should_define_variables() {
    let mut env = Environment::new(None);
    let location = SourceLocation::new_single_line(0, 0, 0);
    let pi_value = LoxObject::Number {
        location,
        value: 3.1415,
    };
    let speed_of_light = LoxObject::Number {
        location,
        value: 299792258.0,
    };


    env.define("PI_VALUE".to_string(), pi_value.clone());
    env.define("SPEED_OF_LIGHT".to_string(), speed_of_light.clone());

    assert_eq!(2, env.length());
    assert_eq!(env.retrieve("PI_VALUE").unwrap(), pi_value);
    assert_eq!(env.retrieve("SPEED_OF_LIGHT").unwrap(), speed_of_light);
}

#[test]
fn it_should_reassign_variables() {
    let mut env = Environment::new(None);
    let location = SourceLocation::new_single_line(0, 0, 0);
    let num1 = LoxObject::Nil {location};
    let num2 = LoxObject::Number {
        location,
        value: 2.71828,
    };

    env.define("number".to_string(), num1.clone());

    assert_eq!(env.retrieve("number").unwrap(), num1);

    env.assign("number".to_string(), num2.clone());

    assert_eq!(env.retrieve("number").unwrap(), num2);
    assert_eq!(1, env.length());
}

