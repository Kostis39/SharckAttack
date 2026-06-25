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
TESTS_FOLDER=tests
UNITY_FOLDER=unity
DOC_FOLDER=docs

CFLAGS=-Wall -Wextra -g -Iinclude -Iunity
SDL_FLAGS=`sdl2-config --cflags`
SDL_LIB=`sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lm

TEST_CFLAGS=$(CFLAGS)
TEST_INCLUDES=-I$(SOURCES_FOLDER) -I$(UNITY_FOLDER)

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

# --- Gestion des tests ---
# Objets pour les tests : tous les .c de src SAUF main.c (Unity fournit son main)
TEST_SRC_OBJECTS=$(filter-out $(OBJECTS_FOLDER)/main.o,$(OBJECTS))
TEST_SOURCES=$(wildcard $(TESTS_FOLDER)/test_*.c)
TEST_OBJECTS=$(patsubst $(TESTS_FOLDER)/%.c,$(OBJECTS_FOLDER)/%.o,$(TEST_SOURCES))
UNITY_OBJECT=$(OBJECTS_FOLDER)/unity.o

# Regle pour compiler unity.c
$(UNITY_OBJECT): $(UNITY_FOLDER)/unity.c create_objects_directory
	$(CC) -c $(TEST_INCLUDES) $(TEST_CFLAGS) -o $@ $<

# Regle pour compiler un test individuel (avec Unity)
$(OBJECTS_FOLDER)/%.o: $(TESTS_FOLDER)/%.c create_objects_directory
	$(CC) -c $(TEST_INCLUDES) $(TEST_CFLAGS) -o $@ $<

# Regle pour generer l'executable d'un test
$(OUTPUT_FOLDER)/test_%: $(OBJECTS_FOLDER)/test_%.o $(UNITY_OBJECT) $(TEST_SRC_OBJECTS)
	if [ ! -d $(OUTPUT_FOLDER) ]; then mkdir $(OUTPUT_FOLDER); fi
	$(CC) $(TEST_SRC_OBJECTS) $< $(UNITY_OBJECT) -o $@ $(SDL_LIB)

# Cible pour compiler tous les tests
test: $(patsubst $(TESTS_FOLDER)/%.c,$(OUTPUT_FOLDER)/%,$(TEST_SOURCES))

# Cible pour lancer tous les tests
run_test: test
	@for test in $(patsubst $(TESTS_FOLDER)/%.c,$(OUTPUT_FOLDER)/%,$(TEST_SOURCES)); do \
		echo "Running $$test"; \
		$$test; \
	done

# Cible pour lancer un test specifique
run_test_%:
	@$(OUTPUT_FOLDER)/test_$*


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