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
	docker run --rm tdbs ./build/tests

FORCE: ;
