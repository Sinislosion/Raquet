
all: build
	cmake . -G Ninja -B build/
	ninja -C build/ -j 20
	./build/Raquet

build:
	mkdir build

clean:
	@rm -rf build
