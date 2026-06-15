#include "bouncer.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>

RGBColor hsv2rgb(float H, float S, float V) {
    float r, g, b;

    float h = H / 360;
    float s = S / 100;
    float v = V / 100;

    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
    case 0:
        r = v, g = t, b = p;
        break;
    case 1:
        r = q, g = v, b = p;
        break;
    case 2:
        r = p, g = v, b = t;
        break;
    case 3:
        r = p, g = q, b = v;
        break;
    case 4:
        r = t, g = p, b = v;
        break;
    case 5:
        r = v, g = p, b = q;
        break;
    }

    RGBColor color;
    color.r = r * 255;
    color.g = g * 255;
    color.b = b * 255;

    return color;
}
void spawnBounce(int size, float baseSpeed) {
    SDL_Window *bouncy = NULL; // Future fenêtre de gauche
    float x = 0.0f, y = 0.0f;
    int running = 1;
    SDL_Event event;
    float velocity[2] = {1, 1};
    SDL_DisplayMode dm;
    int maxX;
    int maxY;
    float speed = baseSpeed;

    float minSpeed = 20.0f;
    float maxSpeed = 200.0f;
    float minSize = 10.0f;
    float maxSize = 500.0f;
    float speedFactor = 0.5f;

    RGBColor color;
    float hue = 0.0;
    Uint32 prevTime = SDL_GetTicks();

    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }
    bouncy = SDL_CreateWindow("bouncing around", 0, 0, size, size,
                              SDL_WINDOW_BORDERLESS);

    SDL_GetCurrentDisplayMode(0, &dm);
    velocity[0] *= speed;
    velocity[1] *= speed;
    maxY = dm.h;
    maxX = dm.w;

    SDL_Renderer *ren =
        SDL_CreateRenderer(bouncy, -1, SDL_RENDERER_ACCELERATED);

    while (running) { // Boucle Principale
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                if (event.key.keysym.sym == SDLK_KP_PLUS)
                    size += 100;
                if (event.key.keysym.sym == SDLK_KP_MINUS)
                    size -= 100;
            }
        }

        // Speed update
        speed = maxSpeed -
                (size - minSize) / (maxSize - minSize) * (maxSpeed - minSpeed);
        if (speed < minSpeed)
            speed = minSpeed;
        speed = speedFactor * speed;
        // Normalisation
        double norm = velocity[0] * velocity[0] + velocity[1] * velocity[1];
        norm = sqrtf(norm);
        x += (velocity[0] / norm) * speed;
        y += (velocity[1] / norm) * speed;

        SDL_SetWindowSize(bouncy, size, size);
        if (x > maxX - (size + 1) || x < 1) {
            velocity[0] *= -1;
        }
        if (y > maxY - (size + 1) || y < 1) {
            velocity[1] *= -1;
        }
        SDL_SetWindowPosition(bouncy, (int)x, (int)y);

        // Time measure
        Uint32 time = SDL_GetTicks();
        float dt = (time - prevTime) / 1000.0f;
        prevTime = time;

        hue += 60.0f * dt;
        if (hue >= 360.0f)
            hue -= 360.0f;

        color = hsv2rgb(hue, 100, 100);
        SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 1);
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
        printf("Window pos: (%d,%d)\n", (int)x, (int)y);
        printf("Bornesup: (%d %d)\n", maxX - (size + 1), maxY - (size + 1));

        printf("Current Size : %dpx Current speed: %f\n\n", size, speed);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(bouncy);
    SDL_Quit();
}
