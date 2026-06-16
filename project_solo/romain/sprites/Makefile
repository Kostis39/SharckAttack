OUTPUT_FOLDER=bin
OBJECTS_FOLDER=build
SOURCES_FOLDER=src
INCLUDES=-I$(SOURCES_FOLDER)
CFLAGS=-Wall -Werror -Wextra -g
SOURCES=$(wildcard $(SOURCES_FOLDER)/*.c)
OBJECTS=$(patsubst $(SOURCES_FOLDER)%.c,$(OBJECTS_FOLDER)%.o,$(SOURCES))

OUTPUT_NAME=sprites

SDL_FLAGS=`sdl2-config --cflags` 
SDL_LIB=`sdl2-config --libs` -lSDL2_image -lm 

link: $(OBJECTS) 
	if [ ! -d $(OUTPUT_FOLDER) ]; then mkdir $(OUTPUT_FOLDER); fi
	$(CC) $(OBJECTS_FOLDER)/*.o  -o $(OUTPUT_FOLDER)/$(OUTPUT_NAME) $(SDL_LIB)
$(OBJECTS_FOLDER)/%.o: $(SOURCES_FOLDER)/%.c create_objects_directory
	$(CC) -c $(INCLUDES) $(CFLAGS) $(SDL_FLAGS) -o $@ $< 

create_objects_directory:
	if [ ! -d $(OBJECTS_FOLDER) ]; then mkdir $(OBJECTS_FOLDER); fi

.PHONY: clean

clean:
	rm -r $(OBJECTS_FOLDER)/ $(OBJECTS_FOLDER)/

run: link
	./$(OUTPUT_FOLDER)/$(OUTPUT_NAME)
