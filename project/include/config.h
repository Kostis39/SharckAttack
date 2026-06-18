#ifndef CONFIG_H
#define CONFIG_H

/* Population */
#define FISH_NB 50

#define RADIUS_SEPARATION 20
#define RADIUS_ALIGNEMENT 35
#define RADIUS_COHESION 40
#define VISION_ANGLE (M_PI * 0.75f)

/* Poids des règles réactives, combinées dans AgentCompute() */
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

#define WIDTH 800
#define HEIGHT 600

#define BORDER_MARGIN 100 // Distance à partir de laquelle la force s'applique
#define TURN_FACTOR 0.2f  // Intensité du virage

#endif /* CONFIG_H */
