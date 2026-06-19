#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265358979323846

/* Population */
#define FISH_NB 200

#define RADIUS_SEPARATION 20
#define RADIUS_ALIGNEMENT 35
#define RADIUS_COHESION 35
#define VISION_ANGLE (M_PI * 0.75f)

/* Poids des règles réactives */
#define SEPARATION 2.5f
#define ALIGNMENT 1.0f
#define COHESION 1.0f

/* Contraintes de mouvement */
#define FISH_SPEED_MAX 2
#define FISH_SPEED_MIN 2

#define SHARK_SPEED_MAX 3

#define TURN_SPEED 0.2f

#define FISH_SIZE 10
#define SHARK_SIZE 22

#define WIDTH 1400
#define HEIGHT 1000

#define BORDER_MARGIN 100 // Distance à partir de laquelle la force s'applique
#define TURN_FACTOR 0.3f  // Intensité du virage

#endif /* CONFIG_H */
