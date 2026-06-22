#include "mj.h"
#include "config.h"

void get_fish_perception(Fish *fish, World *world, FishPerception *perception) {

    perception->self = *fish;

    perception->neighbor_separation = calloc(FISH_NB, sizeof(Fish));
    perception->nb_sep = 0;
    perception->neighbor_alignment = calloc(FISH_NB, sizeof(Fish));
    perception->nb_align = 0;
    perception->neighbor_cohesion = calloc(FISH_NB, sizeof(Fish));
    perception->nb_cohes = 0;

    perception->shark_visible = false;
    perception->width = world->width;
    perception->height = world->height;

    if (world->shark != NULL) {
        Vector to_shark = Vector_sub(world->shark->pos, fish->position);
        float distance = Vector_length2(to_shark);

        if (distance < RADIUS_SHARK_VISIBILITY * RADIUS_SHARK_VISIBILITY) {
            perception->shark_visible = true;
            perception->shark = *world->shark;
        }
    }

    /**Les trois zones de perception sont concentriques et s'enchaînent :
     * [0, separation_limit] -> séparation
     * ]separation_limit, alignment_limit] -> alignement
     * ]alignment_limit, cohesion_limit] -> cohésion
     */
    float separation_limit = fish->radius_separation;
    float alignment_limit = separation_limit + fish->radius_alignement;
    float cohesion_limit = alignment_limit + fish->radius_cohesion;

    for (int i = 0; i < world->nb_fish; i++) {
        Fish *other = &world->fishes[i];

        if (other == fish || !other->is_alive) {
            continue;
        }

        Vector to_other = Vector_sub(other->position, fish->position);
        float distance_squared = Vector_length2(to_other);

        float angle = Vector_angle_fast(fish->velocity, to_other);
        if (fabsf(angle) > fish->vision_angle / 2.0f) {
            continue;
        }

        if (distance_squared <= separation_limit * separation_limit) {
            perception->neighbor_separation[perception->nb_sep] = *other;
            perception->nb_sep++;
        } else if (distance_squared <= alignment_limit * alignment_limit) {
            perception->neighbor_alignment[perception->nb_align] = *other;
            perception->nb_align++;
        } else if (distance_squared <= cohesion_limit * cohesion_limit) {
            perception->neighbor_cohesion[perception->nb_cohes] = *other;
            perception->nb_cohes++;
        }
    }
}

void FishPerception_destroy(FishPerception *perception) {
    free(perception->neighbor_separation);
    free(perception->neighbor_alignment);
    free(perception->neighbor_cohesion);
}

void Get_shark_perception(Shark *shark, World *world,
                          SharkPerception *shark_perception) {

    shark_perception->self = *shark;

    shark_perception->width = world->width;
    shark_perception->height = world->height;

    Vector center = Vector_init();
    Vector avg_vel = Vector_init();
    int count = 0;

    float closest_dist;

    // Parcourir tous les poissons
    for (int i = 0; i < world->nb_fish; i++) {
        Fish *fish = &world->fishes[i];
        if (!fish->is_alive)
            continue;

        // Distance au requin
        Vector to_fish = Vector_sub(fish->position, shark->pos);
        float dist = Vector_length(to_fish);

        // Vérifier si le poisson est dans le champ de vision du requin
        if (dist < SHARK_VISION_RANGE) {
            center = Vector_add(center, fish->position);
            avg_vel = Vector_add(avg_vel, fish->velocity);
            count++;

            // Mettre à jour le poisson le plus proche
            if (dist < closest_dist) {
                closest_dist = dist;
                shark_perception->closest_fish = *fish;
            }
        }
    }

    if (count > 0) {
        shark_perception->center_of_mass = Vector_scale(center, 1.0f / count);
        shark_perception->avg_velocity = Vector_scale(avg_vel, 1.0f / count);

        shark_perception->has_closest_fish = true;

    } else { // Aucun poisson visible
        shark_perception->center_of_mass = Vector_init();
        shark_perception->avg_velocity = Vector_init();

        shark_perception->has_closest_fish = false;
    }
}

void handle_shark_collisions(World *world) {
    Vector shark_pos = world->shark->pos;

    for (int i = 0; i < world->nb_fish; i++) {
        if (!world->fishes[i].is_alive)
            continue;

        Fish *fish = &world->fishes[i];
        float dist = Vector_length(Vector_sub(fish->position, shark_pos));

        if (dist < SHARK_ATTACK_RANGE) {
            fish->is_alive = false;
            world->fish_eaten++;
        }
    }
}

void UpdateWorld(World *world, World *tmp_world) {
    FishPerception *perception = malloc(sizeof(FishPerception));
    for (int i = 0; i < world->nb_fish; i++) {
        // Copie de l'état actuel
        tmp_world->fishes[i] = world->fishes[i];

        if (!world->fishes[i].is_alive) {
            continue;
        }

        get_fish_perception(&world->fishes[i], world, perception);

        Vector action = fish_choose_action(perception, world->rules_set_fish);

        FishPerception_destroy(perception);

        fish_apply_action(&tmp_world->fishes[i], action);
    }
    free(perception);

    // Copie de l'état actuel
    *tmp_world->shark = *world->shark;

    SharkPerception *shark_perception = malloc(sizeof(SharkPerception));
    Get_shark_perception(world->shark, world, shark_perception);

    Vector shark_action = shark_choose_action(shark_perception);

    free(shark_perception);

    Shark_apply_action(tmp_world->shark, shark_action);

    handle_shark_collisions(tmp_world);
}

void Game_step(World *world) {
    World tmp_world;
    tmp_world.width = world->width;
    tmp_world.height = world->height;
    tmp_world.nb_fish = world->nb_fish;
    tmp_world.fish_eaten = world->fish_eaten;

    tmp_world.fishes = calloc(tmp_world.nb_fish, sizeof(Fish));
    if (!tmp_world.fishes) {
        fprintf(stderr, "Erreur malloc dans Game_step (fishes)\n");
        return;
    }

    tmp_world.shark = calloc(1, sizeof(Shark));
    if (!tmp_world.shark) {
        free(tmp_world.fishes);
        fprintf(stderr, "Erreur malloc dans Game_step (shark)\n");
        return;
    }

    UpdateWorld(world, &tmp_world);

    free(world->fishes);
    // Remplacement par le nouveau
    world->fishes = tmp_world.fishes;
    world->nb_fish = tmp_world.nb_fish; // si le nombre a changé

    // Pour le requin, on copie la structure (pas d'échange de pointeur)
    free(world->shark); // 1. Libérer l'ancien requin
    world->shark = tmp_world.shark;
    world->fish_eaten = tmp_world.fish_eaten;
}
