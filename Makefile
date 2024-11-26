# Modify these if you need
COMPILER := clang
CFLAGS := --std=c99 -Wall -Wextra -O2  # Change CFLAGS to C++ flags

# Name of the final executable
TARGET := Raquet

INCLUDES := -Iinclude/ -Iinclude/Raquet

# Build and Source directories.
BUILD_DIR := bin
SRC_DIR := src

ifeq ($(OS), Windows_NT)
    LIBS := -Lwinclude/lib/ -lSDL2 -lSDL2_mixer -lm -lSDL2main -mwindows
    PLATFORM := win
    INSULT := "Windows Dev? I am so sorry."
    EXTENSION := .exe
else
    LIBS := -lSDL2 -lSDL2_mixer -lSDL2main -lm
    PLATFORM := nix
    INSULT := "*Nix Dev? How's your waifu wallpaper holding up?"
    EXTENSION := .x86_64
endif

# Recursive wildcard function
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Find all source files recursively
SRCS_C := $(call rwildcard, $(SRC_DIR), *.c)
# Find C++ files
SRCS_CPP := $(call rwildcard, $(SRC_DIR), *.cpp)
# Round up everything
SRCS := $(SRCS_C) $(SRCS_CPP)

# Create object file paths maintaining directory structure
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS_C)) \
        $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS_CPP))

all: announce $(BUILD_DIR)/ $(TARGET)$(EXTENSION)

# Bash command hell
$(TARGET)$(EXTENSION): $(OBJS)
	@mkdir -p $(BUILD_DIR)/$(PLATFORM)
	@echo $(INSULT)
ifeq ($(OS), Windows_NT)
	@echo
	@echo "Adding icon to executable"
	windres winclude/program.rc -o $(BUILD_DIR)/program.o
	@echo
	@echo "Compiling the final program"
	@$(COMPILER) -o $(BUILD_DIR)/$(PLATFORM)/$(TARGET)$(EXTENSION) $^ $(BUILD_DIR)/program.o $(LIBS)
	@echo
	@echo "Copying DLL Files"
	cp -r winclude/$(BUILD_DIR)/* $(BUILD_DIR)/$(PLATFORM)
else
	@echo
	@echo "Compiling the final program"
	@$(COMPILER) -o $(BUILD_DIR)/$(PLATFORM)/$(TARGET)$(EXTENSION) $^ $(LIBS)
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

# Rule for building object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)  # Create the directory structure in BUILD_DIR
	@$(COMPILER) $(CFLAGS) -c $< $(INCLUDES) -o $@
	@echo "CC	$<" 

# Rule for building C++ object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)  # Create the directory structure in BUILD_DIR
	@$(COMPILER) $(CFLAGS) -c $< $(INCLUDES) -o $@
	@echo "CC	$<"

announce:
	@echo
	@echo "Starting compilation"
	@echo

clean: delete all announce

delete:
	@echo
	@echo "Deleting all build files"
	@rm -rf $(BUILD_DIR)/

.PHONY: all clean delete announce
.NOTPARALLEL: announce clean delete $(BUILD_DIR)/ all
