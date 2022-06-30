# Lox

This repository contains my implementation of the Lox 
Programming Language using C.

This is the result of reading through the first part of
the [Crafting Interpreters](https://www.craftinginterpreters.com/).

## Building

### Dependencies
The project uses the [CMake](https://cmake.org/) build system to 
compile the source code. In addition to that, Makefiles are also
included for ergonomic purposes. 

### Build Process
To compile the project be sure to be on the root directory of the project, 
and run the following command:
```
make compile
```

Alternatively, to compile and run the executable you can do:
```
make lox
```

## Tests
The [Google Testing Framework](https://github.com/google/googletest), was 
used to facilitate the running of tests, to ensure that the code is 
running as intended.

To run the tests, use the following command:
```
make test
```
