#include "world.h"

/**
 * @brief initialise un monde avec des paramètres
 * @param width largeur du monde en pixels
 * @param height hauteur du monde en pixels
 * @param nb_fish nombre de poissons
 * @param nb_colliders nombre d'obstacles
 * @param theta_shark paramètres de la politique du requin
 * @param sigma écart-type pour l'exploration (pour l'apprentissage)
 * @param nb_occurrence nombre maximum de pas par épisode (pour l'apprentissage)
 * @param is_player true si le requin est contrôlé par le joueur
 * @param learn true si en mode apprentissage
 * @return pointeur vers le monde alloué dynamiquement
 */
World *World_init(int width, int height, int nb_fish, int nb_colliders,
                  VectorRule theta_shark, float sigma, int nb_occurrence,
                  bool is_player, bool learn) {
    World *new_world = calloc(1, sizeof(World));
    new_world->width = width;
    new_world->height = height;
    new_world->nb_fish = nb_fish;
    new_world->nb_colliders = nb_colliders;
    new_world->fishes = Fish_create_random_array(nb_fish, width, height);
    new_world->shark = Shark_create(width, height, is_player);
    new_world->colliders =
        Colliders_random_array(width, height, width / COLLIDER_RATIO,
                               height / COLLIDER_RATIO, nb_colliders, 1);
    new_world->fish_eaten = 0;
    new_world->theta_fish = RulesSetFish_init();
    new_world->theta_shark = theta_shark;
    new_world->trajectory = Trajectory_init();
    new_world->sigma = sigma;
    new_world->nb_occurrence = nb_occurrence;
    new_world->learn = learn;

    return new_world;
}

/**
 * @brief libère un monde et tout ses éléments
 * @param world pointeur vers le monde à détruire
 */
void World_destroy(World *world) {
    Fish_destroy_array(world->fishes);
    Shark_destroy(world->shark);
    Colliders_destroy_array(world->colliders);
    RulesSetFish_destroy(world->theta_fish);
    Trajectory_destroy(world->trajectory);
    free(world);
    world = NULL;
}

/**
 * @brief remplace le contenu d'un monde par un autre
 * @param world monde de destination qui sera modifié
 * @param world_tmp monde source copié
 */
void World_replace(World *world, World *world_tmp) {
    Fish_destroy_array(world->fishes);
    world->nb_fish = world_tmp->nb_fish;
    world->fishes = Fish_copy_array(world_tmp->fishes, world_tmp->nb_fish);
    world_tmp->colliders = world_tmp->colliders;
    Shark_copy(world->shark, world_tmp->shark);
}
