#include "mj.h"
#include "collider.h"
#include "config.h"
#include "fish_controller.h"
#include "vector.h"
#include "world.h"

float Shark_reward(int nb_fish_ate) {
    if (nb_fish_ate == 0) {
        return 0;
    }
    return nb_fish_ate;
}

void fish_perception_init(Fish *fish, World *world,
                          FishPerception *perception) {

    /**
     * @brief Initialise la perception d'un poisson
     * @param fish le poisson à initialiser
     * @param world le monde contenant les dimensions
     * @param perception la structure de perception à remplir
     */
    perception->self = *fish;

    perception->collider_separation = Vector_init();
    perception->separation = Vector_init();
    perception->center_of_mass = Vector_init();
    perception->avg_velocity = Vector_init();
    perception->dist_shark = Vector_init();

    perception->shark_visible = false;
    perception->width = world->width;
    perception->height = world->height;
}

void fish_to_shark_perception(Fish *fish, World *world,
                              FishPerception *perception) {

    /**
     * @brief Met à jour la perception du requin pour un poisson
     * @param fish le poisson dont on met à jour la perception
     * @param world le monde contenant le requin
     * @param perception la structure de perception à mettre à jour
     */
    if (world->shark != NULL) {
        Vector to_shark = Vector_sub(world->shark->pos, fish->position);
        float distance = Vector_length2(to_shark);

        if (distance < RADIUS_SHARK_VISIBILITY * RADIUS_SHARK_VISIBILITY) {
            perception->shark_visible = true;
            perception->shark = *world->shark;
        }
    }
    if (perception->shark_visible) {
        perception->dist_shark =
            Vector_sub(perception->self.position, perception->shark.pos);
    }
}

void fish_neighbor_perception(Fish *fish, World *world,
                              FishPerception *perception, int *nb_align,
                              int *nb_cohes) {
    /**
     * @brief fish_neighbor_perception donne la percpetion des voisins de fish
     * @param fish le poisson
     * @param world le monde
     * @param perception la perception actuelle du poisson
     * @param nb_align pointeur vers le nombre de poissons dans la zone
     * d'alignement
     * @param nb_align pointeur vers le nombre de poissons dans la zone de
     * cohésion
     * */

    float separation_limit = fish->radius_separation;
    float alignment_limit = separation_limit + fish->radius_alignement;
    float cohesion_limit = alignment_limit + fish->radius_cohesion;
    for (int i = 0; i < world->nb_fish; i++) {
        Fish *neighbor = &world->fishes[i];

        if (neighbor == fish || !neighbor->is_alive) {
            continue;
        }

        Vector to_neighbor = Vector_sub(neighbor->position, fish->position);
        float distance_squared = Vector_length2(to_neighbor);

        float angle = Vector_angle(fish->velocity, to_neighbor);
        if (fabsf(angle) > fish->vision_angle / 2.0f) {
            continue;
        }

        if (distance_squared <= separation_limit * separation_limit) {
            perception->separation = Vector_add(
                perception->separation,
                Vector_sub(perception->self.position, neighbor->position));
        } else if (distance_squared <= alignment_limit * alignment_limit) {
            perception->avg_velocity =
                Vector_add(perception->avg_velocity, neighbor->velocity);
            (*nb_align)++;
        } else if (distance_squared <= cohesion_limit * cohesion_limit) {
            perception->center_of_mass =
                Vector_add(perception->center_of_mass, neighbor->position);
            (*nb_cohes)++;
        }
    }
}

void get_fish_perception(Fish *fish, World *world, FishPerception *perception) {
    /**
     * @brief renvoie la perception d'un poisson dans le monde
     * @param fish le poisson en question
     * @param world le monde dans lequel évolue le poisson
     * @param perception la struct dans laquelle on écrit la perception associée
     * au poisson fish
     * */

    fish_perception_init(fish, world, perception);
    fish_to_shark_perception(fish, world, perception);
    int nb_cohes = 0;
    int nb_align = 0;
    fish_neighbor_perception(fish, world, perception, &nb_align, &nb_cohes);

    /**Les trois zones de perception sont concentriques et s'enchaînent :
     * [0, separation_limit] -> séparation
     * ]separation_limit, alignment_limit] -> alignement
     * ]alignment_limit, cohesion_limit] -> cohésion
     */

    float max_perception = fish->radius_separation;

    for (int j = 0; j < world->nb_colliders; ++j) {
        Collider *c = &world->colliders[j];
        Vector closest = Collider_closest_point(c, fish->position);
        float dist = Vector_distance(fish->position, closest);

        if (dist < max_perception && dist > 0.0f) {
            float intensity = 1.0f - (dist / max_perception);
            Vector repulsion =
                Vector_scale(Vector_sub(fish->position, closest), intensity);
            perception->collider_separation =
                Vector_add(perception->collider_separation, repulsion);
        }
    }
    perception->avg_velocity =
        (nb_align > 0) ? Vector_scale(perception->avg_velocity, 1.0f / nb_align)
                       : Vector_init();

    perception->center_of_mass =
        (nb_cohes > 0)
            ? Vector_scale(perception->center_of_mass, 1.0f / nb_cohes)
            : perception->self.position;
}

void FishPerception_destroy(FishPerception *perception) { free(perception); }

void Get_shark_perception(Shark *shark, World *world,
                          SharkPerception *shark_perception) {

    shark_perception->self = *shark;

    shark_perception->width = world->width;
    shark_perception->height = world->height;

    Vector center = Vector_init();
    Vector avg_vel = Vector_init();
    int count = 0;

    float closest_dist = INFINITY;

    // Parcourir tous les poissons
    for (int i = 0; i < world->nb_fish; i++) {
        Fish *fish = &world->fishes[i];
        if (!fish->is_alive)
            continue;

        // Distance au requin
        Vector to_fish = Vector_sub(fish->position, shark->pos);
        float dist = Vector_length(to_fish);

        // Mettre à jour le poisson le plus proche
        if (dist < closest_dist) {
            closest_dist = dist;
            shark_perception->closest_fish = *fish;
        }

        // Calculs pour les poisson proche
        if (dist < SHARK_VISION_RANGE) {
            center = Vector_add(center, fish->position);
            avg_vel = Vector_add(avg_vel, fish->velocity);
            count++;
        }
    }

    if (count > 0) {
        shark_perception->center_of_mass = Vector_scale(center, 1.0f / count);
        shark_perception->avg_velocity = Vector_scale(avg_vel, 1.0f / count);

        shark_perception->has_prey_visible = true;

    } else { // Aucun poisson visible
        shark_perception->center_of_mass = Vector_init();
        shark_perception->avg_velocity = Vector_init();

        shark_perception->has_prey_visible = false;
    }
    shark_perception->has_prey = closest_dist != INFINITY;
}

/**
 * @brief Calcule combien de poissons sont mangés, et les mange.
 *
 * @param world Le monde où ce passe l'action.
 * @return int Le nombre de poisson mangé.
 */
int handle_shark_eat(World *world) {
    Vector shark_pos = world->shark->pos;
    int has_eaten = 0;

    for (int i = 0; i < world->nb_fish; i++) {
        if (!world->fishes[i].is_alive)
            continue;

        Fish *fish = &world->fishes[i];
        float dist = Vector_length(Vector_sub(fish->position, shark_pos));

        if (dist < SHARK_ATTACK_RANGE) {
            fish->is_alive = false;
            world->fish_eaten++;
            has_eaten += 1;
        }
    }
    return has_eaten;
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

        fish_apply_action(&tmp_world->fishes[i], action);
    }
    FishPerception_destroy(perception);

    // Copie de l'état actuel
    *tmp_world->shark = *world->shark;

    SharkPerception *shark_perception = malloc(sizeof(SharkPerception));
    Get_shark_perception(world->shark, world, shark_perception);

    Vector shark_action =
        shark_choose_action(shark_perception, world->rules_set_shark);

    free(shark_perception);

    Shark_apply_action(tmp_world->shark, shark_action);

    int nb_fish_ate = handle_shark_eat(tmp_world);
    int reward = Shark_reward(nb_fish_ate);

    Need_trajectory_growing(world->trajectory);
    Add_step(world->trajectory, world->shark->pos, shark_action, reward);
}

void Game_step(World *world) {
    /**
     * @brief réalise une itération du jeu
     * @param world le monde à itérer
     * */
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
