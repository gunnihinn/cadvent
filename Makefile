build/02: src/02.c build
	$(CC) -o build/02 -ggdb src/02.c

build/01: src/01.c build
	$(CC) -o build/01 src/01.c

format:
	clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4}" -i src/01.c src/02.c

build:
	mkdir -p build

.PHONY: clean
clean:
	rm -rf build
