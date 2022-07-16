
.PHONY: src build examples include tests

all: compile setup test

compile: setup
	$(MAKE) -s -C "build" 

setup:
	cmake -S . -B build

lox: setup compile
	"./build/clox"

clean:
	rm -rf "external"
	rm -rf "build"

test: compile
	"./build/test_runner"

debug: compile
	gdb ./build/clox

debug_test: compile
	gdb ./build/test_runner

leak: compile
	valgrind --track-origins=yes --leak-check=full "./build/clox" "./examples/hello_world.lox"
