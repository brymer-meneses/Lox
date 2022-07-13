
.PHONY: treewalker bytecode

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
