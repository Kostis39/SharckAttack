CC=gcc

OUTPUT_NAME=sprites3

CFLAGS=-Wall -Wextra -g -Iinclude
SDL_FLAGS=$(shell sdl2-config --cflags --libs)
LIBS=-lSDL2_image -lSDL2_ttf

SRC=src/sprites3_main.c \
    src/sprites_utils.c \
    src/scene.c \
    src/player.c \
    src/enemy.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT_NAME) $(SDL_FLAGS) $(LIBS)

run: all
	./$(OUTPUT_NAME)

clean:
	rm -f $(OUTPUT_NAME)

.PHONY: all run clean