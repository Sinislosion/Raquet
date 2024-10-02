CC := clang
CFLAGS := -Wall -Wextra -O2
TARGET := Raquet

IFLAGS := -Iinclude/

ifeq ($(OS), Windows_NT)
		LFLAGS := -Lwinclude/lib/ -lSDL2 -lSDL2_mixer -lm -lSDL2main -mwindows
		PLATFORM := win
		INSULT := "Windows Dev? I am so sorry."
		EXTENSION := .exe
else
		LFLAGS := `sdl2-config --libs` -lSDL2_mixer -lSDL2main -lm
		PLATFORM := nix
		INSULT := "*Nix Dev? How's your waifu wallpaper holding up?"
		EXTENSION := .x86_64
endif

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,bin/%.o,$(SRCS))

all: announce bin/ $(TARGET)
$(TARGET): $(OBJS)
		@mkdir -p bin/$(PLATFORM)
		@echo $(INSULT)
ifeq ($(OS), Windows_NT)
		@echo
		@echo "Adding icon to executable"
		windres winclude/program.rc -o bin/program.o
		@echo
		@echo "Compiling the final program"
		$(CC) -o bin/$(PLATFORM)/$@ $^ bin/program.o $(LFLAGS)
		@echo
		@echo "Copying DLL Files"
		cp -r winclude/bin/* bin/$(PLATFORM)
else
		@echo
		@echo "Compiling the final program"
		$(CC) -o bin/$(PLATFORM)/$@ $^ $(LFLAGS)
endif
		@echo
		@echo "Copying assets"
		cp -r assets/ bin/$(PLATFORM)
		@echo
		@echo "Running $(TARGET)"
		./bin/$(PLATFORM)/$(TARGET)

bin/:
	@echo "No build directory, creating one now"
	mkdir -p bin

bin/%.o: src/%.c
		$(CC) $(CFLAGS) -c $< $(IFLAGS) -o bin/$(patsubst src/%.c,%.o,$<)

announce:
		@echo
		@echo "Starting compilation"
		@echo

clean: delete all announce

delete:
		@echo
		@echo "Deleting all build files"
		@rm -rf bin/*.o
	
.PHONY: all clean delete announce
.NOTPARALLEL: announce clean delete bin/ all