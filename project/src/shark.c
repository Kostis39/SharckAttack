#include "shark.h"

Shark *Shark_createRandom(int width, int height) {
    Shark *shark = calloc(1, sizeof(Shark));
    shark->pos.x = rand() % width;
    shark->pos.y = rand() % height;
    shark->velocity.x = (rand() % SHARK_SPEED_MAX) - (SHARK_SPEED_MAX / 2);
    shark->velocity.y = (rand() % SHARK_SPEED_MAX) - (SHARK_SPEED_MAX / 2);
    return shark;
}

void Shark_destroy(Shark *shark) {
    free(shark);
    shark = NULL;
}