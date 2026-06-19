#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265358979323846

/* Population */
#define FISH_NB 200

#define RADIUS_SEPARATION 32
#define RADIUS_ALIGNEMENT 160
#define RADIUS_COHESION 160
#define VISION_ANGLE (M_PI * 0.75f)

/* Poids des règles réactives */
#define SEPARATION 0.3f
#define ALIGNMENT 0.3f
#define COHESION 0.003f

/* Contraintes de mouvement */
#define FISH_SPEED_MAX 4.0f
#define FISH_SPEED_MIN 2.0f

#define SHARK_SPEED_MAX 3

#define TURN_SPEED 0.2f

#define FISH_SIZE 10
#define SHARK_SIZE 22

#define WIDTH 1400
#define HEIGHT 1000

#define REPULSION_ZONE                                                         \
    100 // Taille des la zone de répulsion (marges de l'écran)
#define REPULSION_FACTOR 2.0f // Intensité du virage

#endif /* CONFIG_H */
