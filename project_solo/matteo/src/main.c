#include "bouncer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
/* #include <stdio.h> */

#define NUM_WINDOWS 5

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

    /* Tableau de fenêtres */
    BouncingWindow_t *windows[NUM_WINDOWS];
    int initialSize = 500;
    int sizeStep = 40;
    for (int i = 0; i < NUM_WINDOWS; i++) {
        int size = initialSize - i * sizeStep;
        if (size < 50)
            size = 50;
        windows[i] = spawnBounce(size, 5, maxX, maxY);
        /* Décalage temporel : décale la position initiale de chaque fenêtre */
        windows[i]->x += (float)(i * 120);
        windows[i]->y += (float)(i * 80);
    }

    SDL_Renderer *ren =
        SDL_CreateRenderer(windows[0]->bouncer, -1, SDL_RENDERER_ACCELERATED);
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
                    windows[0]->size += 100;
                if (event.key.keysym.sym == SDLK_KP_MINUS)
                    windows[0]->size -= 100;
            }
        }
        for (int i = 0; i < NUM_WINDOWS; i++) {
            windows[i] = UpdateBounce(windows[i]);
        }
        renderRGB = *RenderBounce(&renderRGB);
    }

    SDL_DestroyRenderer(ren);
    for (int i = 0; i < NUM_WINDOWS; i++) {
        SDL_DestroyWindow(windows[i]->bouncer);
        free(windows[i]);
    }
    SDL_Quit();
    return 0;
}
