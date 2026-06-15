#include "bouncer.h"

void spawnBounce(int size) {
    SDL_Window *bouncy = NULL; // Future fenêtre de gauche
    int x = 0, y = 0;
    int running = 1;
    SDL_Event event;
    float velocity[2] = {10, 10};
    int maxX;
    int maxY;
    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }
    bouncy = SDL_CreateWindow("bouncing around", 0, 0, size, size,
                              SDL_WINDOW_BORDERLESS);

    SDL_GL_GetDrawableSize(bouncy, &maxX, &maxY);
    maxY = 1080;
    maxX = 1920;

    while (running) { // Boucle Principale
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN &&
                 event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }

        x += velocity[0];
        y += velocity[1];
        if (x > maxX - (size + 1) || x < 1) {
            velocity[0] *= -1;
        }
        if (y > maxY - (size + 1) || y < 1) {
            velocity[1] *= -1;
        }
        SDL_SetWindowPosition(bouncy, x, y);

        printf("Window pos: (%d,%d)\n", x, y);
        printf("Bornesup: %d \n\n", maxX - (size + 1));
    }
    SDL_DestroyWindow(bouncy);
    SDL_Quit();
}
