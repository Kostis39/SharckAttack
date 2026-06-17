#ifndef SDLDISPPLAY_H
#define SDLDISPPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "cell.h"
#include "mj.h"

typedef struct SDLdisplay {
    SDL_Window *window;
    SDL_Renderer *renderer;

    int windowWidth;
    int windowHeight;

    float zoom;
    float offsetX;
    float offsetY;

    int 
}

#endif
