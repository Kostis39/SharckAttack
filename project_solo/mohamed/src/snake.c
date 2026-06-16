#include "snake.h"

#include <stdlib.h>
#include <time.h>

static int random_between(int min, int max) {
    return min + rand() % (max - min + 1);
}

static int random_component(void) {
    int value = random_between(0, 2);

    if (value == 0) {
        return -1;
    }

    if (value == 1) {
        return 0;
    }

    return 1;
}

static void choose_valid_direction(Snake *snake) {
    do {
        snake->dx = random_component();
        snake->dy = random_component();
    } while (snake->dx == 0 && snake->dy == 0);
}

static int safe_random_x(int width) {
    if (width < 160) {
        return width / 2;
    }

    return random_between(80, width - 80);
}

static int safe_random_y(int height) {
    if (height < 160) {
        return height / 2;
    }

    return random_between(80, height - 80);
}

static void init_one_snake(Snake *snake, int width, int height) {
    int start_x = safe_random_x(width);
    int start_y = safe_random_y(height);

    choose_valid_direction(snake);

    snake->red = (unsigned char)random_between(80, 255);
    snake->green = (unsigned char)random_between(80, 255);
    snake->blue = (unsigned char)random_between(80, 255);

    snake->step_counter = 0;
    snake->turn_period = random_between(25, 80);

    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake->body[i].x = start_x - i * snake->dx * SNAKE_SIZE;
        snake->body[i].y = start_y - i * snake->dy * SNAKE_SIZE;
    }
}

static void keep_head_inside(Snake *snake, int width, int height) {
    if (snake->body[0].x < SNAKE_SIZE) {
        snake->body[0].x = SNAKE_SIZE;
        snake->dx = -snake->dx;
    }

    if (snake->body[0].x > width - SNAKE_SIZE) {
        snake->body[0].x = width - SNAKE_SIZE;
        snake->dx = -snake->dx;
    }

    if (snake->body[0].y < SNAKE_SIZE) {
        snake->body[0].y = SNAKE_SIZE;
        snake->dy = -snake->dy;
    }

    if (snake->body[0].y > height - SNAKE_SIZE) {
        snake->body[0].y = height - SNAKE_SIZE;
        snake->dy = -snake->dy;
    }
}

void init_snakes(Snake snakes[], int count, int width, int height) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < count; i++) {
        init_one_snake(&snakes[i], width, height);
    }
}

void randomize_snakes(Snake snakes[], int count, int width, int height) {
    for (int i = 0; i < count; i++) {
        init_one_snake(&snakes[i], width, height);
    }
}

void update_snakes(Snake snakes[], int count, int width, int height, int speed) {
    for (int i = 0; i < count; i++) {
        Snake *snake = &snakes[i];

        for (int j = SNAKE_LENGTH - 1; j > 0; j--) {
            snake->body[j] = snake->body[j - 1];
        }

        snake->step_counter++;

        if (snake->step_counter >= snake->turn_period) {
            choose_valid_direction(snake);
            snake->step_counter = 0;
            snake->turn_period = random_between(25, 80);
        }

        snake->body[0].x += snake->dx * speed;
        snake->body[0].y += snake->dy * speed;

        keep_head_inside(snake, width, height);
    }
}
