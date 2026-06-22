#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265358979323846

/* Population */
#define FISH_NB 200

#define RADIUS_SEPARATION 32
#define RADIUS_ALIGNEMENT 160
#define RADIUS_COHESION 160
#define RADIUS_SHARK_VISIBILITY 200
#define VISION_ANGLE (PI * 0.25f)

#define SHARK_VISION_RANGE 300

/* Poids des règles réactives */
#define SEPARATION 0.3f
#define ALIGNMENT 0.3f
#define COHESION 0.003f
#define SHARK_AVOIDANCE_FACTOR 0.03f

/* Contraintes de mouvement */
#define FISH_SPEED_MAX 4.0f
#define FISH_SPEED_MIN 2.0f

#define SHARK_SPEED_MAX 5

#define TURN_SPEED 0.2f

#define FISH_SIZE 10
#define SHARK_SIZE 22

#define WIDTH 800
#define HEIGHT 800

#define REPULSION_ZONE                                                         \
    100 // Taille des la zone de répulsion (marges de l'écran)
#define REPULSION_FACTOR 2.0f // Intensité du virage

#define NB_OCCURRENCE 10000

#define RANDOM_SEED 42

#define COLLIDER_RATIO                                                         \
    10 /* mesure au plus un dixième de la largeur du                          \
          monde*/
#define COLLIDERS_NB 5

#define SHARK_ATTACK_RANGE 30
#endif /* CONFIG_H */
