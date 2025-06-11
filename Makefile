all: build
	cmake . -G Ninja -B build/
	ninja -C build/ -j 20
ifeq ($(OS),Windows_NT)
	
else
	@echo "Nix"
endif
	./build/Raquet

build:
	mkdir build

build/windeps:
ifeq ($(OS),Windows_NT)
	mkdir build/windeps

	wget https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-devel-2.8.1-mingw.zip -O build/windeps/mixer.zip
	unzip build/windeps/mixer.zip -d build/windeps
	cp build/windeps/SDL2_mixer-2.8.1/x86_64-w64-mingw32/bin/SDL2_mixer.dll build/

	wget https://github.com/libsdl-org/SDL/releases/download/release-2.32.6/SDL2-devel-2.32.6-mingw.zip -O build/windeps/sdl2.zip
	unzip build/windeps/sdl2.zip -d build/windeps
	cp build/windeps/SDL2-2.32.6/x86_64-w64-mingw32/bin/SDL2.dll build/

endif

package: build/windeps
ifeq ($(OS),Windows_NT)
	zip -j Release.zip build/Raquet.exe build/SDL2.dll build/SDL2_mixer.dll
	zip -r Release.zip assets
endif

clean:
	@rm -rf build
