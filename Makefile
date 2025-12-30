all: build run

build: FORCE
	mkdir build -p
	cd build && CC=clang CXX=clang++ && cmake .. && cmake --build .

run: build
	./build/tdbs

test: build
	cd build && ctest --output-on-failure

valgrind: build
	cd build && valgrind --leak-check=full --track-origins=yes ./tdbs

clean:
	rm -rf build

FORCE: ;
