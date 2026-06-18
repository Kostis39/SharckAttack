CC=gcc

OUTPUT_NAME=serpent

CFLAGS=-Wall -Wextra -g -Iinclude
SDL_FLAGS=$(shell sdl2-config --cflags --libs)
LIBS=-lm

SRC=src/serpent_main.c \
    src/snake.c \
    src/draw.c \
    src/window_tools.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT_NAME) $(SDL_FLAGS) $(LIBS)

run: all
	./$(OUTPUT_NAME)

clean:
	rm -f $(OUTPUT_NAME)

.PHONY: all run clean
