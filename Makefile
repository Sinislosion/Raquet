# Modify these if you need
COMPILER := gcc
CFLAGS := --std=c99 -Wall -Wextra -O2

#Name of the final executable
TARGET := Raquet

IFLAGS := -Iinclude/*

# build and source directories. no you cannot have 2 files with the same name
# in seperate recursive directories.
BUILD_DIR := bin
SRC_DIR := src

ifeq ($(OS), Windows_NT)
		LFLAGS := -Lwinclude/lib/ -lSDL2 -lSDL2_mixer -lm -lSDL2main -mwindows
		PLATFORM := win
		INSULT := "Windows Dev? I am so sorry."
		EXTENSION := .exe
else
		LFLAGS := -lSDL2 -lSDL2_mixer -lSDL2main -lm
		PLATFORM := nix
		INSULT := "*Nix Dev? How's your waifu wallpaper holding up?"
		EXTENSION := .x86_64
endif

# some guy on stack overflow made this. i am forever in his debt
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS := $(call rwildcard, $(SRC_DIR), *.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

all: announce $(BUILD_DIR)/ $(TARGET)$(EXTENSION)
$(TARGET)$(EXTENSION): $(OBJS)
		@mkdir -p $(BUILD_DIR)/$(PLATFORM)
		@echo $(INSULT)
ifeq ($(OS), Windows_NT)
		@echo
		@echo "Adding icon to executable"
		windres winclude/program.rc -o $(BUILD_DIR)/program.o
		@echo
		@echo "Compiling the final program"
		$(COMPILER) -o $(BUILD_DIR)/$(PLATFORM)/$(TARGET)$(EXTENSION) $(addprefix $(BUILD_DIR)/, $(notdir $^)) $(BUILD_DIR)/program.o $(LFLAGS)
		@echo
		@echo "Copying DLL Files"
		cp -r winclude/$(BUILD_DIR)/* $(BUILD_DIR)/$(PLATFORM)
else
		@echo
		@echo "Compiling the final program"
		$(COMPILER) -o $(BUILD_DIR)/$(PLATFORM)/$(TARGET)$(EXTENSION) $^ $(LFLAGS)
endif
		@echo
		@echo "Copying assets"
		cp -r assets/ $(BUILD_DIR)/$(PLATFORM)
		@echo
		@echo "Running $(TARGET)"
		./$(BUILD_DIR)/$(PLATFORM)/$(TARGET)$(EXTENSION)

$(BUILD_DIR)/:
	@echo "No build directory, creating one now"
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
		$(COMPILER) $(CFLAGS) -c $< $(IFLAGS) -o $(BUILD_DIR)/$(notdir $(patsubst $(SRC_DIR)/%.c,%.o,$<))

announce:
		@echo
		@echo "Starting compilation"
		@echo

clean: delete all announce

delete:
		@echo
		@echo "Deleting all build files"
		@rm -rf $(BUILD_DIR)/*.o

.PHONY: all clean delete announce
.NOTPARALLEL: announce clean delete $(BUILD_DIR)/ all