.PHONY: config build run test valgrind clean docker-build docker-run docker-test

all: config build run

config:
	mkdir build -p
	cd build && cmake ..

config-ubuntu:
	mkdir build -p
	cd build && cmake .. \
		-DCMAKE_C_COMPILER=clang \
		-DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_FLAGS="--gcc-toolchain=/usr --gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13"

build:
	cd build && cmake --build .

run:
	./build/tdbs

test: build
	cd build && ctest --output-on-failure

valgrind: build
	cd build && valgrind --leak-check=full --track-origins=yes ./tdbs

clean:
	rm -rf build

docker-build:
	docker build -t tdbs .

docker-run: docker-build
	xhost +local:docker
	docker run --rm \
		-e DISPLAY=$(DISPLAY) \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		-v $(XAUTHORITY):/root/.Xauthority:ro \
		--network host \
		tdbs
	xhost -local:docker

docker-test: docker-build
	docker run --rm tdbs ./tests
