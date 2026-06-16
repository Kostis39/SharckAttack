#ifndef SNAKE_H
#define SNAKE_H

#include "config.h"

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point body[SNAKE_LENGTH];

    int dx;
    int dy;

    unsigned char red;
    unsigned char green;
    unsigned char blue;

    int step_counter;
    int turn_period;
} Snake;

void init_snakes(Snake snakes[], int count, int width, int height);
void randomize_snakes(Snake snakes[], int count, int width, int height);
void update_snakes(Snake snakes[], int count, int width, int height, int speed);

#endif
