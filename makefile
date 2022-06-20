
.PHONY: treewalker bytecode

compile: setup
	$(MAKE) -s -C "build" 

setup:
	cmake -S . -B build

lox: setup compile
	"./build/loxTW"

clean:
	rm -rf "external"
	rm -rf "build"

test: compile
	"./build/test_runner"
