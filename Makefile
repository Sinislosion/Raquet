PROGRAMNAME = RAQUET

EXTENSION = .x86_64
CFLAGS := -Wall -O2
IFLAGS := -Iinclude/
LFLAGS := `sdl2-config --libs` -lSDL2_mixer -lSDL2main -lm
PLATFORM := nix

CARGS := -std=c99 -O2

W_EXTENSION = .exe
W_CFLAGS := -O2 -DWINDOWS
W_LFLAGS := -Lwinclude/lib/ -Iinclude/ -Iwinclude/include -lSDL2 -lSDL2_mixer -lSDL2main -mwindows
W_PLATFORM := win

COMPILER := clang

start: bin/Raquet.o bin/main.o
		
ifeq ($(OS), Windows_NT)
		@echo "Windows Dev? I am so sorry."
		@echo "Compiling $(PROGRAMNAME)"
		$(COMPILER) $(CARGS) bin/Raquet.o bin/main.o -o bin/$(W_PLATFORM)/$(PROGRAMNAME)$(W_EXTENSION) $(W_CFLAGS) $(IFLAGS) $(W_LFLAGS)
		cp -r winclude/bin/* bin/$(W_PLATFORM)
		cp -r assets bin/$(W_PLATFORM)
		./bin/$(W_PLATFORM)/$(PROGRAMNAME)$(W_EXTENSION)
else
		@echo "*Nix Dev? How's your waifu wallper holding up?"
		mkdir -p bin
		mkdir -p bin/$(PLATFORM)
		@echo "Compiling $(PROGRAMNAME)"
		$(COMPILER) $(CARGS) -o bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION) $(CFLAGS) $(IFLAGS) $(LFLAGS)
		chmod u+x bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION)
		cp -r assets bin/$(PLATFORM)
		./bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION)
endif

bin/Raquet.o:
		mkdir -p bin
		mkdir -p bin/$(W_PLATFORM)
		$(COMPILER) $(CARGS) include/Raquet.c -c -o bin/Raquet.o $(IFLAGS) $(W_CFLAGS)
		
bin/main.o:
		$(COMPILER) $(CARGS) src/main.c -c -o bin/main.o $(IFLAGS) $(W_CFLAGS)

clean:
ifeq ($(OS), Windows_NT)
		rm -r bin/$(W_PLATFORM)
		@echo "Erased from History"
else
		rm -r bin/$(PLATFORM)
		@echo "Erased from History"
endif
