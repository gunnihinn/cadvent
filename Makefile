build/01: src/01.c build
	$(CC) -o build/01 src/01.c

build:
	mkdir -p build

.PHONY: clean
clean:
	rm -rf build
