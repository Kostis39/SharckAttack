#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define SCROLL_SPEED 4

typedef struct {
    SDL_Texture *texture;
    int w, h;
} Tex;

void end_sdl(char ok,                                               // fin normale : ok = 0 ; anormale ok = 1
             char const* msg,                                       // message à afficher
             SDL_Window* window,                                    // fenêtre à fermer
             SDL_Renderer* renderer) {                              // renderer à fermer
    char msg_formated[255];                                      
    int l;                                         

    if (!ok) {                                                        // Affichage de ce qui ne va pas
        strncpy(msg_formated, msg, 250);                                   
        l = strlen(msg_formated);                                      
        strcpy(msg_formated + l, " : %s\n");                                 
        SDL_Log(msg_formated, SDL_GetError());                                 
    }                                            

    if (renderer != NULL) {                                           // Destruction si nécessaire du renderer
        SDL_DestroyRenderer(renderer);                                  
        renderer = NULL;
    }
    if (window != NULL)   {                                           // Destruction si nécessaire de la fenêtre
        SDL_DestroyWindow(window);                                      
        window= NULL;
    }

    SDL_Quit();                                          

    if (!ok) {                             // On quitte si cela ne va pas            
        exit(EXIT_FAILURE);                                        
    }                                            
}

SDL_Texture* load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Surface *my_image = NULL;           // Variable de passage
    SDL_Texture *my_texture = NULL;         // La texture

    my_image = IMG_Load(file_image_name);   // Chargement de l'image dans la surface
    if (my_image == NULL)
        end_sdl(0, "Chargement de l'image impossible", window, renderer);

    my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);
    if (my_texture == NULL)
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

    return my_texture;
}


void draw_texture_repeat(Tex t, int offset, SDL_Renderer *renderer, int y_pos) {
    int start = (t.w - (offset % t.w)) % t.w;   // Décalage avec l'offset, sans sortir de l'intervalle

    for (int x = start - t.w; x < WINDOW_WIDTH; x += t.w) { // Position de toutes les images
        SDL_Rect dst = {x, y_pos, t.w, t.h};
        SDL_RenderCopy(renderer, t.texture, NULL, &dst);
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        end_sdl(0, "ERROR SDL INIT", NULL, NULL);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        end_sdl(0, "ERROR IMG INIT", NULL, NULL);

    SDL_Window *window = SDL_CreateWindow("Sol défilant",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL)
        end_sdl(0, "ERROR WINDOW CREATION", NULL, NULL);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        end_sdl(0, "ERROR RENDERER CREATION", window, NULL);

    // Chargement de la texture du sol
    Tex ground;
    ground.texture = load_texture_from_image("assets/ground.png", window, renderer);
    SDL_QueryTexture(ground.texture, NULL, NULL, &ground.w, &ground.h);

    int ground_y = WINDOW_HEIGHT - ground.h;   // Pour mettre l'image en bas
    int scroll_offset = 0;

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        scroll_offset += SCROLL_SPEED;

        // Rendu
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Couleur du ciel
        SDL_RenderClear(renderer);
        draw_texture_repeat(ground, scroll_offset, renderer, ground_y);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_DestroyTexture(ground.texture);
    end_sdl(1, "Fin normale", window, renderer);
    return EXIT_SUCCESS;
}