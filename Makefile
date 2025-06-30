# OPTIONS #
GAME_NAME := Raquet
CC := clang

ifeq ($(OS),Windows_NT)
	INSULT := "Windows Dev? I am so sorry."
else
	INSULT := "*Nix Dev? How's your waifu wallpaper holding up?"
endif

# CODE #

all: build
	@echo $(INSULT)
	@echo
	@echo "Generating CMake Project"
	@echo
	@cmake . -G Ninja -B build/ -DGAME_NAME=$(GAME_NAME) -DCMAKE_C_COMPILER=$(CC)
	@echo
	@echo "Running Ninja"
	@echo
	@ninja -C build/ -j 20
ifeq ($(OS),Windows_NT)
	@cp ./build/_deps/sdl3_mixer-build/SDL3_mixer.dll ./build/

endif
	@echo
	@echo "Running $(GAME_NAME)"
	@echo
	@./build/$(GAME_NAME)

build:
	@echo "Creating Build Directory"
	@mkdir build

build/windeps:
ifeq ($(OS),Windows_NT)
	@echo "Creating Windows Dependency Directory"
	@mkdir build/windeps
	@echo "Downloading SDL3"
	@wget -q https://github.com/libsdl-org/SDL/releases/download/release-3.2.16/SDL3-devel-3.2.16-mingw.zip -O build/windeps/sdl3.zip
	@echo "Unzipping SDL3"
	@unzip -q build/windeps/sdl3.zip -d build/windeps
	@echo "Copying to the build folder"
	@cp build/windeps/SDL3-3.2.16/x86_64-w64-mingw32/bin/SDL3.dll build/

endif

package: build/windeps
ifeq ($(OS),Windows_NT)
	@zip -qj Release.zip build/$(GAME_NAME).exe build/SDL3.dll build/SDL3_mixer.dll
	@zip -qr Release.zip assets
endif

clean: removebuilddir all

removebuilddir:
	@rm -rf build
