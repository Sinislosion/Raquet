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
	@echo
	@echo "Running $(GAME_NAME)"
	@echo
	@./build/$(GAME_NAME)

build:
	@echo "Creating Build Directory"
	@mkdir build

package:
ifeq ($(OS),Windows_NT)
	@zip -qj Release.zip build/$(GAME_NAME).exe build/SDL3.dll build/SDL3_mixer.dll
	@zip -qr Release.zip assets
else
	@zip -qj Release.zip build/$(GAME_NAME) build/_deps/sdl3-build/libSDL3.so.0.* build/_deps/sdl3_mixer-build/libSDL3_mixer.so.0.*
	@zip -qr Release.zip assets
endif

clean: removebuilddir all

removebuilddir:
	@rm -rf build
