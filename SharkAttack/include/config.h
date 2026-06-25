#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265358979323846

enum Rules {
    Rules_Center = 0,
    Rules_Alignment,
    Rules_Pursuit,
    Rules_zone_density,
    Rules_Lenght
};

#define FILE_LOG "logs.txt"
/* Poids des règles de shark*/
#define THETA_FILE "params.txt"

#define STEP_LOG 100

#define NB_OCCURRENCE 10000

#define TRAJECTORY_LENGHT_ALLOC 200
#define TRAJECTORY_CAPACITY NB_OCCURRENCE

/* Population */
#define FISH_NB 100

#define RADIUS_SEPARATION 32
#define RADIUS_ALIGNEMENT 160
#define RADIUS_COHESION 160
#define RADIUS_SHARK_VISIBILITY 200
#define VISION_ANGLE (PI * 0.25f)

#define SHARK_VISION_RANGE 500

/* Poids des règles de fish */
#define SEPARATION 0.3f
#define ALIGNMENT 0.3f
#define COHESION 0.003f
#define SHARK_AVOIDANCE 0.1f
#define COLLIDER_AVOIDANCE 0.9f

/* Contraintes de mouvement */
#define FISH_SPEED_MAX 4.0f
#define FISH_SPEED_MIN 2.0f

#define SHARK_SPEED_MAX 5

#define TURN_SPEED 0.2f

#define FISH_SIZE 10
#define SHARK_SIZE 22

#define WIDTH 1920
#define HEIGHT 1080

#define REPULSION_ZONE 10 // Taille des la zone de répulsion (marges de l'écran)
#define REPULSION_FACTOR 10.0f // Intensité du virage

#define RANDOM_SEED 42

#define COLLIDER_RATIO                                                         \
    25 /* mesure au plus un dixième de la largeur du                          \
          monde*/
#define COLLIDERS_NB 5

#define SHARK_ATTACK_RANGE 30

#define BENCHMARK_ITERATIONS 2000

#define NB_THREADS 10

#define AUDIO_PATH "assets/"

#endif /* CONFIG_H */
