#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "sprites_config.h"

typedef struct {
    SDL_Rect dst;

    float y_float;
    float velocity_y;

    int facing_right;
    int grounded;

    int current_frame;
    int current_row;

    SDL_bool attacking;
    Uint32 attack_start_time;
    int attack_already_hit;
} Player;

void init_player(Player *player,
                 int frame_w,
                 int frame_h);

void start_jump(Player *player);

void start_attack(Player *player);

void update_player(Player *player,
                   const Uint8 *keyboard,
                   int *moving);

void update_attack(Player *player);

void update_animation(Player *player,
                      int moving,
                      Uint32 *last_frame_time);

SDL_Rect get_attack_box(Player player);

void draw_player(SDL_Renderer *renderer,
                 SDL_Texture *texture,
                 Player player,
                 int frame_w,
                 int frame_h);

#endif