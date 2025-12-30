all: build run

build: FORCE
	mkdir build -p
	cd build && cmake .. && cmake --build .

run: build
	cd build && ./app

test: build
	cd build && ctest --output-on-failure

valgrind: build
	cd build && valgrind --leak-check=full --track-origins=yes ./app

clean:
	rm -rf build

FORCE: ;
