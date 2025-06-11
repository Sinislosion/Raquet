# OPTIONS #
GAME_NAME := Raquet
CC := clang

ifeq ($(OS),Windows_NT)
	INSULT := "Windows Dev? I am so sorry."
else
	INSULT := "*Nix Dev? How's your waifu wallpaper holding up?
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
	@echo "Downloading SDL2_Mixer"
	@wget -q https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-devel-2.8.1-mingw.zip -O build/windeps/mixer.zip
	@echo "Unzipping SDL2_Mixer"
	@unzip -q build/windeps/mixer.zip -d build/windeps
	@echo "Copying to the build folder"
	@cp build/windeps/SDL2_mixer-2.8.1/x86_64-w64-mingw32/bin/SDL2_mixer.dll build/
	@echo
	@echo "Downloading SDL2"
	@wget -q https://github.com/libsdl-org/SDL/releases/download/release-2.32.6/SDL2-devel-2.32.6-mingw.zip -O build/windeps/sdl2.zip
	@echo "Unzipping SDL2"
	@unzip -q build/windeps/sdl2.zip -d build/windeps
	@echo "Copying to the build folder"
	@cp build/windeps/SDL2-2.32.6/x86_64-w64-mingw32/bin/SDL2.dll build/

endif

package: build/windeps
ifeq ($(OS),Windows_NT)
	@zip -qj Release.zip build/$(GAME_NAME).exe build/SDL2.dll build/SDL2_mixer.dll
	@zip -qr Release.zip assets
endif

clean: removebuilddir all

removebuilddir:
	@rm -rf build
