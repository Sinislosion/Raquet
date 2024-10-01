PROGRAMNAME = RAQUET

CARGS := -std=c99 -O2 -Iinclude/

ifeq ($(OS), Windows_NT)
		EXTENSION = .exe
		CFLAGS := -O2 -DWINDOWS
		LFLAGS := -Lwinclude/lib/ -Iwinclude/include -lSDL2 -lSDL2_mixer -lSDL2main -mwindows
		PLATFORM := win

		INSULT := "Windows Dev? I am so sorry."
else
		EXTENSION = .x86_64
		CFLAGS := -Wall -O2
		IFLAGS := -Iinclude/
		LFLAGS := `sdl2-config --libs` -lSDL2_mixer -lSDL2main -lm
		PLATFORM := nix

		INSULT := "*Nix Dev? How's your waifu wallpaper holding up?"
endif

COMPILER := clang

all: bin/Raquet.o bin/Raquet_Math.o bin/main.o
		@echo $(INSULT)
		@echo "Compiling $(PROGRAMNAME)"
		$(COMPILER) $(CARGS) bin/Raquet.o bin/Raquet_Math.o bin/main.o -o bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION) $(CFLAGS) $(IFLAGS) $(LFLAGS)
ifeq ($(OS), Windows_NT)
		cp -r winclude/bin/* bin/$(PLATFORM)
endif
		cp -r assets bin/$(PLATFORM)
		./bin/$(PLATFORM)/$(PROGRAMNAME)$(EXTENSION)


bin/Raquet.o:
		mkdir -p bin
		mkdir -p bin/$(PLATFORM)
		$(COMPILER) $(CARGS) include/Raquet.c -c -o bin/Raquet.o $(IFLAGS) $(W_CFLAGS)

bin/Raquet_Math.o:
		$(COMPILER) $(CARGS) include/Raquet_Math.c -c -o bin/Raquet_Math.o $(IFLAGS) $(W_CFLAGS)

bin/main.o:
		$(COMPILER) $(CARGS) src/main.c -c -o bin/main.o $(IFLAGS) $(W_CFLAGS)

clean:
		rm -r bin/*
		@echo "Erased from History"
		make
