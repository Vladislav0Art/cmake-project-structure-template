.PHONY: run
run:
	./build/main

.PHONY: build
build:
	mkdir -p ./build
	cd ./build && cmake -DCMAKE_PREFIX_PATH=/Users/Vladislav.Artiukhov/.local .. && make