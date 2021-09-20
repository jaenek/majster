.PHONY: all clean configure compile

all: configure compile

clean:
	rm -rf build

build:
	mkdir -p build
	cd build && CC=clang cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	cp build/compile_commands.json src/

configure: build

compile: build
	cd build && make
