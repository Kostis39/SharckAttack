#ifndef CONFIG_H
#define CONFIG_H

/* Population */
#define FISH_NB 80

#define PERCEPTION_RADIUS                                                      \
    60.0f /* rayon de perception générale (alignement, cohésion) */
#define SEPARATION_RADIUS 20.0f /* rayon plus court, déclenche la répulsion */
#define MAX_VOISINS 32          /* taille max du tableau de voisins perçus */

/* Poids des règles réactives, combinées dans AgentCompute() */
#define POIDS_SEPARATION 1.5f
#define POIDS_ALIGNEMENT 1.0f
#define POIDS_COHESION 1.0f

/* Contraintes de mouvement */
#define FISH_SPEED_MAX 150.0f
#define FISH_SPEED_MIN 40.0f

#define SHARK_SPEED_MAX 150.0f
#define SHARK_SPEED_MIN 40.0f

#define FISH_SIZE 10
#define SHARK_SIZE 22

#endif /* CONFIG_H */