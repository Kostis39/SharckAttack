CC=gcc

OUTPUT_NAME=x_fenetre2

CFLAGS=-Wall -Wextra -g
SDL_FLAGS=$(shell sdl2-config --cflags --libs)

SRC=src/X_fenetre_main2.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT_NAME) $(SDL_FLAGS)

run: all
	./$(OUTPUT_NAME)

clean:
	rm -f $(OUTPUT_NAME)

.PHONY: all run clean