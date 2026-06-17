#include "enemy.h"

void draw_enemy(SDL_Renderer *renderer,
                SDL_Texture *enemy_texture,
                SDL_Rect enemy,
                int enemy_life) {
    if (enemy_life <= 0) {
        return;
    }

    if (enemy_texture != NULL) {
        SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy);
    } else {
        SDL_SetRenderDrawColor(renderer, 180, 50, 50, 255);
        SDL_RenderFillRect(renderer, &enemy);
    }
}

void draw_life_bar(SDL_Renderer *renderer,
                   SDL_Rect enemy,
                   int life_value) {
    SDL_Rect border = {
        enemy.x - 10,
        enemy.y - 25,
        120,
        12
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &border);

    SDL_Rect life = {
        enemy.x - 10,
        enemy.y - 25,
        life_value,
        12
    };

    SDL_SetRenderDrawColor(renderer, 0, 220, 80, 255);
    SDL_RenderFillRect(renderer, &life);
}