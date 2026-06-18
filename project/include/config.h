#ifndef CONFIG_H
#define CONFIG_H

/* Population */
#define FISH_NB 10

#define RADIUS_SEPARATION 20
#define RADIUS_ALIGNEMENT 40
#define RADIUS_COHESION 60
#define VISION_ANGLE (M_PI * 0.75f)

/* Poids des règles réactives, combinées dans AgentCompute() */
#define SEPARATION 1.5f
#define ALIGNMENT 1.0f
#define COHESION 1.0f

/* Contraintes de mouvement */
#define FISH_SPEED_MAX 40

#define SHARK_SPEED_MAX 45

#define FISH_SIZE 10
#define SHARK_SIZE 22

#define WIDTH 500
#define HEIGHT 500

#endif /* CONFIG_H */
