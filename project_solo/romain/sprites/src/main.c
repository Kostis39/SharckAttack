#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LARGEUR_FENETRE  800
#define HAUTEUR_FENETRE  600

#define VITESSE_DEFILEMENT 4

typedef struct {
    SDL_Texture *texture;
    int w, h;
} Tex;

// Fonction de fermeture
static void end_sdl(bool ok, char const *msg,
                    SDL_Window *window, SDL_Renderer *renderer) {
    if (!ok) SDL_Log("%s : %s\n", msg, SDL_GetError());
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    if (!ok) exit(EXIT_FAILURE);
}

// Chargement d'une texture depuis une image
static SDL_Texture* load_texture_from_image(char *file_image_name,
                                            SDL_Window *window,
                                            SDL_Renderer *renderer) {
    SDL_Surface *my_image = IMG_Load(file_image_name);
    if (!my_image)
        end_sdl(false, "Chargement de l'image impossible", window, renderer);

    SDL_Texture *my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);
    if (!my_texture)
        end_sdl(false, "Echec de la transformation en texture", window, renderer);

    return my_texture;
}

// Dessine le sol en répétant la texture avec un décalage
void draw_texture_repeat(Tex t, int offset, SDL_Renderer *renderer, int y_pos) {
    int start = (t.w - (offset % t.w)) % t.w; // Décalage initial

    for (int x = start - t.w; x < LARGEUR_FENETRE; x += t.w) {
        SDL_Rect dst = {x, y_pos, t.w, t.h};
        SDL_RenderCopy(renderer, t.texture, NULL, &dst);
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        end_sdl(false, "SDL_Init", NULL, NULL);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        end_sdl(false, "IMG_Init", NULL, NULL);

    SDL_Window *window = SDL_CreateWindow("Sol défilant",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          LARGEUR_FENETRE, HAUTEUR_FENETRE,
                                          SDL_WINDOW_SHOWN);
    if (!window)
        end_sdl(false, "SDL_CreateWindow", NULL, NULL);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        end_sdl(false, "SDL_CreateRenderer", window, NULL);

    // Chargement du sol
    Tex ground;
    ground.texture = load_texture_from_image("assets/ground.png", window, renderer);
    SDL_QueryTexture(ground.texture, NULL, NULL, &ground.w, &ground.h);

    int ground_y = HAUTEUR_FENETRE - ground.h;
    int scroll_offset = 0;
    int direction = 0;   // -1 : gauche, +1 : droite, 0 : arrêt

    bool program_on = true;
    SDL_Event event;

    while (program_on) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    program_on = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            direction = -1;
                            break;
                        case SDLK_d:
                            direction =  1;
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            direction = 0;
                            break;
                        case SDLK_d:
                            direction = 0;
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        // Mise à jour du décalage
        scroll_offset += direction * VITESSE_DEFILEMENT;

        // Rendu
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Couleur du ciel
        SDL_RenderClear(renderer);
        draw_texture_repeat(ground, scroll_offset, renderer, ground_y);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_DestroyTexture(ground.texture);
    end_sdl(true, "Fin normale", window, renderer);
    return EXIT_SUCCESS;
}