#include "bouncer.h"

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
BouncingWindow_t *spawnBounce(int size, float baseSpeed, int maxX, int maxY) {

    BouncingWindow_t *bouncy = malloc(sizeof(*bouncy));
    bouncy->bouncer =
        SDL_CreateWindow("bouncing", 0, 0, size, size, SDL_WINDOW_BORDERLESS);
    bouncy->speed = baseSpeed;
    bouncy->size = size;
    bouncy->velocity[0] *= bouncy->speed;
    bouncy->velocity[1] *= bouncy->speed;
    bouncy->maxY = maxX;
    bouncy->maxY = maxY;
    bouncy->x = 0.0f;
    bouncy->y = 0.0f;
    bouncy->velocity[0] = 1;
    bouncy->velocity[1] = 1;
    bouncy->minSpeed = 20.0f;
    bouncy->maxSpeed = 200.0f;
    bouncy->minSize = 10.0f;
    bouncy->maxSize = 500.0f;
    bouncy->speedFactor = 0.5f;

    return bouncy;
}

RendererBounce_t *RenderBounce(RendererBounce_t *ren) {
    // ************ Render Operations ************//

    // Time measure
    Uint32 time = SDL_GetTicks();
    float dt = (time - ren->prevTime) / 1000.0f;
    ren->prevTime = time;

    ren->hue += 60.0f * dt;
    if (ren->hue >= 360.0f)
        ren->hue -= 360.0f;

    ren->color = hsv2rgb(ren->hue, 100, 100);
    SDL_SetRenderDrawColor(ren->ren, ren->color.r, ren->color.g, ren->color.b,
                           1);
    SDL_RenderClear(ren->ren);
    SDL_RenderPresent(ren->ren);
    return ren;
}

BouncingWindow_t *UpdateBounce(BouncingWindow_t *bounce) {

    // ************ Window operations ************//
    // Speed update
    bounce->speed =
        bounce->maxSpeed - (bounce->size - bounce->minSize) /
                               (bounce->maxSize - bounce->minSize) *
                               (bounce->maxSpeed - bounce->minSpeed);
    if (bounce->speed < bounce->minSpeed)
        bounce->speed = bounce->minSpeed;
    bounce->speed = bounce->speedFactor * bounce->speed;
    // Normalisation
    double norm = bounce->velocity[0] * bounce->velocity[0] +
                  bounce->velocity[1] * bounce->velocity[1];
    norm = sqrtf(norm);
    bounce->x += (bounce->velocity[0] / norm) * bounce->speed;
    bounce->y += (bounce->velocity[1] / norm) * bounce->speed;

    SDL_SetWindowSize(bounce->bouncer, bounce->size, bounce->size);
    if (bounce->x > bounce->maxX - (bounce->size + 1) || bounce->x < 1) {
        bounce->velocity[0] *= -1;
    }
    if (bounce->y > bounce->maxY - (bounce->size + 1) || bounce->y < 1) {
        bounce->velocity[1] *= -1;
    }
    SDL_SetWindowPosition(bounce->bouncer, (int)bounce->x, (int)bounce->y);
    // ************ END ************//
    return bounce;
}
/* void spawnBounce_old(int size, float baseSpeed) { */

/*     // Window variables */
/*     SDL_Window *bouncy = NULL; // Future fenêtre de gauche */
/*     float x = 0.0f, y = 0.0f; */
/*     float velocity[2] = {1, 1}; */
/*     // Speed Variables */
/*     float speed = baseSpeed; */
/*     float minSpeed = 20.0f; */
/*     float maxSpeed = 200.0f; */
/*     float minSize = 10.0f; */
/*     float maxSize = 500.0f; */
/*     float speedFactor = 0.5f; */

/*     // Render variables */
/*     RGBColor color; */
/*     float hue = 0.0; */
/*     Uint32 prevTime = SDL_GetTicks(); */

/*     int running = 1; */
/*     SDL_Event event; */
/*     bouncy = SDL_CreateWindow("bouncing around", 0, 0, size, size, */
/*                               SDL_WINDOW_BORDERLESS); */

/*     velocity[0] *= speed; */
/*     velocity[1] *= speed; */

/*     SDL_Renderer *ren = */
/*         SDL_CreateRenderer(bouncy, -1, SDL_RENDERER_ACCELERATED); */

/*     while (running) { // Boucle Principale */

/*         // ********* Event Operations*********\// */
/*         while (SDL_PollEvent(&event)) { */
/*             if (event.type == SDL_KEYDOWN) { */
/*                 if (event.key.keysym.sym == SDLK_ESCAPE) */
/*                     running = 0; */
/*                 if (event.key.keysym.sym == SDLK_KP_PLUS) */
/*                     size += 100; */
/*                 if (event.key.keysym.sym == SDLK_KP_MINUS) */
/*                     size -= 100; */
/*             } */
/*         } */
/*         // ************ END ************\// */

/*         // ************ Window operations ************\// */
/*         // Speed update */
/*         speed = maxSpeed - */
/*                 (size - minSize) / (maxSize - minSize) * (maxSpeed -
 * minSpeed); */
/*         if (speed < minSpeed) */
/*             speed = minSpeed; */
/*         speed = speedFactor * speed; */
/*         // Normalisation */
/*         double norm = velocity[0] * velocity[0] + velocity[1] * velocity[1];
 */
/*         norm = sqrtf(norm); */
/*         x += (velocity[0] / norm) * speed; */
/*         y += (velocity[1] / norm) * speed; */

/*         SDL_SetWindowSize(bouncy, size, size); */
/*         if (x > maxX - (size + 1) || x < 1) { */
/*             velocity[0] *= -1; */
/*         } */
/*         if (y > maxY - (size + 1) || y < 1) { */
/*             velocity[1] *= -1; */
/*         } */
/*         SDL_SetWindowPosition(bouncy, (int)x, (int)y); */
/*         // ************ END ************\// */

/*         // ************ Render Operations ************\// */
/*         // Time measure */
/*         Uint32 time = SDL_GetTicks(); */
/*         float dt = (time - prevTime) / 1000.0f; */
/*         prevTime = time; */

/*         hue += 60.0f * dt; */
/*         if (hue >= 360.0f) */
/*             hue -= 360.0f; */

/*         color = hsv2rgb(hue, 100, 100); */
/*         SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 1); */
/*         SDL_RenderClear(ren); */
/*         SDL_RenderPresent(ren); */

/*         // ********* Debug Operations *********\// */
/*         printf("Window pos: (%d,%d)\n", (int)x, (int)y); */
/*         printf("Bornesup: (%d %d)\n", maxX - (size + 1), maxY - (size + 1));
 */

/*         printf("Current Size : %dpx Current speed: %f\n\n", size, speed); */
/*     } */
/* } */
