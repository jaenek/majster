.PHONY: all clean configure run

all: clean run

clean:
	rm -rf build/bin/main

build:
	rm -rf build/bin
	mkdir -p build
	cd build && CC=clang cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	cp build/compile_commands.json src/

# alias for build
configure: build

build/bin/main: build
	cd build && make

run: build/bin/main
	./$?
