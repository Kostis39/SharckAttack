#include "shark.h"

Shark *Shark_createRandom(int width, int height) {
    Shark *shark = calloc(1, sizeof(Shark));
    shark->pos.x = rand() % width;
    shark->pos.y = rand() % height;
    shark->velocity.x = (rand() % SHARK_SPEED_MAX) - (SHARK_SPEED_MAX / 2);
    shark->velocity.y = (rand() % SHARK_SPEED_MAX) - (SHARK_SPEED_MAX / 2);
    return shark;
}

int Shark_copy(Shark *shark_dest, Shark *shark_src) {
    if (shark_dest == NULL || shark_src == NULL) {
        return -1; // Error: Null pointer
    }
    shark_dest->pos = shark_src->pos;
    shark_dest->velocity = shark_src->velocity;
    return 0;
}

void Shark_destroy(Shark *shark) {
    free(shark);
    shark = NULL;
}