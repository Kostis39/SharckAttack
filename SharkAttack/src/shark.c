#include "shark.h"
#include "render_sdl.h"

/**
 * @brief crée un requin avec une position initiale et un mode joueur ou bot
 * @param width largeur du monde
 * @param height hauteur du monde
 * @param is_player true si le requin est contrôlé par le joueur, false pour un
 * bot
 * @return pointeur vers le requin alloué dynamiquement
 */
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

/**
 * @brief copie les données d'un requin source vers un requin destination
 * @param shark_dest pointeur vers le requin de destination
 * @param shark_src pointeur vers le requin source
 * @return 0 si succès, -1 sinon
 */
int Shark_copy(Shark *shark_dest, Shark *shark_src) {
    if (shark_dest == NULL || shark_src == NULL) {
        return -1; // Error: Null pointer
    }
    shark_dest->pos = shark_src->pos;
    shark_dest->velocity = shark_src->velocity;
    return 0;
}

/**
 * @brief libère la mémoire d'un requin
 * @param shark pointeur vers le requin à libérer
 */
void Shark_destroy(Shark *shark) {
    free(shark);
    shark = NULL;
}

/**
 * @brief vérifie si le requin est contrôlé par le joueur
 * @param shark pointeur vers le requin
 * @return true si le requin est un joueur, false sinon
 */

bool Is_player(Shark *shark) { return shark->player; }

/**
 * @brief applique une action au requin et met à jour sa position
 * @param shark pointeur vers le requin
 * @param action vecteur vitesse à appliquer
 * @param width largeur du monde
 * @param height hauteur du monde
 */
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
    if (shark->pos.x > width)
        shark->pos.x = width;
    if (shark->pos.y < 0.0f)
        shark->pos.y = 0.0f;
    if (shark->pos.y > height)
        shark->pos.y = height;
}
