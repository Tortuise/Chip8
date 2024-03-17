CC = gcc
CFLAGS = -Wall -Wextra
SDL_LIB = -lmingw32 -lSDL2main -lSDL2

# Source files
CHIP8_SRC = chip8.c
INSTRUCTIONS_SRC = instructions.c

# Object files
CHIP8_OBJ = chip8.o
INSTRUCTIONS_OBJ = instructions.o

# Headers
CHIP8_HEADER = chip8.h
INSTRUCTIONS_HEADER = instructions.h

# Executable
TARGET = chip8_emulator

all: $(TARGET)

$(TARGET): $(CHIP8_OBJ) $(INSTRUCTIONS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_LIB)

$(CHIP8_OBJ): $(CHIP8_SRC) $(CHIP8_HEADER) $(INSTRUCTIONS_HEADER)
	$(CC) $(CFLAGS) -c -o $@ $<

$(INSTRUCTIONS_OBJ): $(INSTRUCTIONS_SRC) $(INSTRUCTIONS_HEADER) $(CHIP8_HEADER)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.exe

.PHONY: all clean