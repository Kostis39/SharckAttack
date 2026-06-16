#include "bouncer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
/* #include <stdio.h> */

/************************************/
/*  exemple de création de fenêtres */
/************************************/

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    /* SDL_Window *window_1 = NULL, // Future fenêtre de gauche */
    /*     *window_2 = NULL;        // Future fenêtre de droite */

    /* /\* Initialisation de la SDL  + gestion de l'échec possible *\/ */
    /* if (SDL_Init(SDL_INIT_VIDEO) != 0) { */
    /*     SDL_Log("Error : SDL initialisation - %s\n", */
    /*             SDL_GetError()); // l'initialisation de la SDL a échoué */
    /*     exit(EXIT_FAILURE); */
    /* } */

    /* /\* Création de la fenêtre de gauche *\/ */
    /* window_1 = SDL_CreateWindow( */
    /*     "Fenêtre à gauche",    // codage en utf8, donc accents possibles */
    /*     0, 0,                  // coin haut gauche en haut gauche de l'écran
     */
    /*     400, 300,              // largeur = 400, hauteur = 300 */
    /*     SDL_WINDOW_RESIZABLE); // redimensionnable */

    /* if (window_1 == NULL) { */
    /*     SDL_Log("Error : SDL window 1 creation - %s\n", */
    /*             SDL_GetError()); // échec de la création de la fenêtre */
    /*     SDL_Quit();              // On referme la SDL */
    /*     exit(EXIT_FAILURE); */
    /* } */
    /* SDL_Delay(2000); // Pause exprimée  en ms */

    /* /\* et on referme tout ce qu'on a ouvert en ordre inverse de la création
     * *\/ */
    /* SDL_DestroyWindow(window_2); // la fenêtre 2 */
    /* SDL_DestroyWindow(window_1); // la fenêtre 1 */

    /* SDL_Quit(); // la SDL */

    // Exécution de Xwindow

    SDL_Event event;
    int maxX;
    int maxY;
    int running = 1;
    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    maxY = dm.h;
    maxX = dm.w;

    /* spawnBounce(500, 5); */
    BouncingWindow_t *bouncy = spawnBounce(500, 5, maxX, maxY);

    SDL_Renderer *ren =
        SDL_CreateRenderer(bouncy->bouncer, -1, SDL_RENDERER_ACCELERATED);
    RendererBounce_t renderRGB;
    renderRGB.prevTime = SDL_GetTicks();
    renderRGB.ren = ren;
    renderRGB.hue = 0.0f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                if (event.key.keysym.sym == SDLK_KP_PLUS)
                    bouncy->size += 100;
                if (event.key.keysym.sym == SDLK_KP_MINUS)
                    bouncy->size -= 100;
            }
        }
        bouncy = UpdateBounce(bouncy);
        renderRGB = *RenderBounce(&renderRGB);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(bouncy->bouncer);
    free(bouncy);
    SDL_Quit();
    return 0;
}
