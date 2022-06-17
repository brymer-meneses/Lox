
.PHONY: treewalker bytecode

compile: setup
	$(MAKE) -s -C "build" 

setup:
	cmake -S . -B build

treewalker: setup compile
	"./build/cloxTW"

clean:
	rm -rf "external"
	rm -rf "build"

test: compile
	"./build/test_runner"
