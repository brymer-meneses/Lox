all: setup compile run

setup:
	cmake -S . -B build

compile:
	cd "build" && make

run: compile
	cd "build" && "./clox"

clean:
	rm -rf "build"
