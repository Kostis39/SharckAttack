# make | compile et link
# make TARGET=cible | compile cible.c
# make clean | supprime les dossier de build
# make run | lance le programme en mode SDL
# make run-term | lance le programme en mode terminal
# make run-learn | lance le programme en mode apprentissage
# make doc | génère la documentation avec Doxygen

OUTPUT_FOLDER=bin
OBJECTS_FOLDER=build
SOURCES_FOLDER=src
DOC_FOLDER=docs

CFLAGS=-Wall -Wextra -g -Iinclude -Iunity
SDL_FLAGS=`sdl2-config --cflags`
SDL_LIB=`sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

INCLUDES=-I$(SOURCES_FOLDER)

TARGET ?= all

# --- Compilation principale ---
ifeq ($(TARGET),all)
SOURCES=$(wildcard $(SOURCES_FOLDER)/*.c)
else
SOURCES=$(SOURCES_FOLDER)/$(TARGET).c
endif

OBJECTS=$(patsubst $(SOURCES_FOLDER)/%.c,$(OBJECTS_FOLDER)/%.o,$(SOURCES))
OUTPUT_NAME=Sharkattack

all: run

link: $(OBJECTS)
	if [ ! -d $(OUTPUT_FOLDER) ]; then mkdir $(OUTPUT_FOLDER); fi
	$(CC) $(OBJECTS) -o $(OUTPUT_FOLDER)/$(OUTPUT_NAME) $(SDL_LIB)

$(OBJECTS_FOLDER)/%.o: $(SOURCES_FOLDER)/%.c create_objects_directory
	$(CC) -c $(INCLUDES) $(CFLAGS) $(SDL_FLAGS) -o $@ $<

create_objects_directory:
	if [ ! -d $(OBJECTS_FOLDER) ]; then mkdir $(OBJECTS_FOLDER); fi

# --- Lancement du programme ---
run: link
	./$(OUTPUT_FOLDER)/$(OUTPUT_NAME)
 
run-term: link
	./$(OUTPUT_FOLDER)/$(OUTPUT_NAME) term
 
run-learn: link
	./$(OUTPUT_FOLDER)/$(OUTPUT_NAME) learn

# --- Documentation Doxygen ---
doc:
	doxygen Doxyfile
	@echo "Documentation generee dans ./$(DOC_FOLDER)"

doc-clean:
	rm -rf $(DOC_FOLDER)/

.PHONY: clean test run_test all link run run-term run-learn doc doc-clean

clean:
	rm -rf $(OUTPUT_FOLDER)/ $(OBJECTS_FOLDER)/
