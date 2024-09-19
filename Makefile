PROGRAMNAME = RAQUET

EXTENSION = .x86_64
CFLAGS := -Wall -O2
LFLAGS := -Iinclude/ `sdl2-config --libs` -lSDL2_mixer 
PLATFORM := nix

CARGS := -std=c99 -O2 src/main.c

W_EXTENSION = .exe
W_CFLAGS := -O2 -DWINDOWS
W_LFLAGS := -Lwinclude/lib/ -Iinclude/ -Iwinclude/include -lSDL2 -lSDL2_mixer -lSDL2main -mwindows
W_PLATFORM := win

COMPILER := clang

start: src/main.c
		
ifeq ($(OS), Windows_NT)
		@echo "Windows Dev? I am so sorry."
		mkdir -p bin
		mkdir -p bin/$(W_PLATFORM)
		@echo "Compiling $(PROGRAMNAME)"
		$(COMPILER) $(CARGS) -o bin/$(W_PLATFORM)/$(PROGRAMNAME)$(W_EXTENSION) $(W_CFLAGS) $(W_LFLAGS)
		cp -r winclude/bin/* bin/$(W_PLATFORM)
		cp -r assets bin/$(W_PLATFORM)
		./bin/$(W_PLATFORM)/$(PROGRAMNAME)$(W_EXTENSION)
else
		@echo "*Nix Dev? How's your waifu wallper holding up?"
		mkdir -p bin
		mkdir -p bin/$(PLATFORM)
		@echo "Compiling $(PROGRAMNAME)"
		$(COMPILER) -o bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION) $(CFLAGS) $(LFLAGS)
		chmod u+x bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION)
		cp -r assets bin/$(PLATFORM)
		./bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION)
endif

clean:
ifeq ($(OS), Windows_NT)
		rm -r bin/$(W_PLATFORM)
		@echo "Erased from History"
else
		rm -r bin/$(PLATFORM)
		@echo "Erased from History"
endif
