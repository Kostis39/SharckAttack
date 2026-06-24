#include "shark.h"
#include "render_sdl.h"

Shark *Shark_create(int width, int height, bool is_player) {
    Shark *shark = calloc(1, sizeof(Shark));
    shark->pos.x = width / 2.0f;
    shark->pos.y = height;
    shark->velocity.x = (SHARK_SPEED_MAX / 2.0f);
    shark->velocity.y = (SHARK_SPEED_MAX / 2.0f);
    shark->radius_vision = SHARK_VISION_RANGE;
    shark->player = is_player;
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

bool Is_player(Shark *shark) { return shark->player; }

void Shark_apply_action(Shark *shark, Vector action, int width, int height) {
    shark->velocity =
        Vector_add(shark->velocity, Vector_scale(action, TURN_SPEED));

    float speed = Vector_length(shark->velocity);
    if (speed > SHARK_SPEED_MAX) {
        shark->velocity =
            Vector_scale(Vector_normalize(shark->velocity), SHARK_SPEED_MAX);
    }

    shark->pos = Vector_add(shark->pos, shark->velocity);

    if (shark->pos.x < 0.0f)
        shark->pos.x = 0.0f;
    if (shark->pos.x > WIDTH)
        shark->pos.x = WIDTH;
    if (shark->pos.y < 0.0f)
        shark->pos.y = 0.0f;
    if (shark->pos.y > HEIGHT)
        shark->pos.y = HEIGHT;
}
