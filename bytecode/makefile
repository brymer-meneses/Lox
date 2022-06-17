all: setup compile run


setup: dependencies
	cmake -S . -B build

compile:
	cd "build" && make

run: compile
	cd "build" && "./clox"

clean:
	rm -rf "build"

test:
	cd "build" && make test
