#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "sprites_config.h"
#include "sprites_utils.h"
#include "scene.h"
#include "player.h"
#include "enemy.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Texture *background = NULL;
    SDL_Texture *player_texture = NULL;
    SDL_Texture *enemy_texture = NULL;

    TTF_Font *font = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) &
         (IMG_INIT_PNG | IMG_INIT_JPG)) == 0) {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("FIGHT",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Erreur fenetre : %s\n", SDL_GetError());
        clean(window, renderer, background, player_texture, enemy_texture, font);
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        printf("Erreur renderer : %s\n", SDL_GetError());
        clean(window, renderer, background, player_texture, enemy_texture, font);
        return EXIT_FAILURE;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    background = load_texture(renderer,
                              "assets/backgrounds/background.jpg");

    player_texture = load_texture(renderer,
                                  "assets/sprites/player-spritemap-v9.png");

    enemy_texture = load_texture(renderer,
                                 "assets/sprites/enemy.png");

    font = TTF_OpenFont("assets/fonts/Pacifico.ttf", 22);

    int sheet_w = 8 * 32;
    int sheet_h = 4 * 32;

    if (player_texture != NULL) {
        SDL_QueryTexture(player_texture, NULL, NULL, &sheet_w, &sheet_h);
    }

    int frame_w = sheet_w / SPRITE_COLUMNS;
    int frame_h = sheet_h / SPRITE_ROWS;

    Player player;
    init_player(&player, frame_w, frame_h);

    SDL_Rect enemy;
    enemy.w = 130;
    enemy.h = 150;
    enemy.x = WINDOW_WIDTH - 230;
    enemy.y = WINDOW_HEIGHT - GROUND_HEIGHT - enemy.h;

    int enemy_life = 100;

    SDL_bool running = SDL_TRUE;
    SDL_Event event;

    Uint32 last_frame_time = SDL_GetTicks();

    int scroll_x = 0;

    while (running) {
        int moving = 0;

        /********************************************/
        /*              Gestion des événements       */
        /********************************************/

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = SDL_FALSE;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_q ||
                    event.key.keysym.sym == SDLK_ESCAPE) {
                    running = SDL_FALSE;
                }

                if (event.key.keysym.sym == SDLK_z ||
                    event.key.keysym.sym == SDLK_UP) {
                    start_jump(&player);
                }

                if (event.key.keysym.sym == SDLK_f ||
                    event.key.keysym.sym == SDLK_SPACE) {
                    start_attack(&player);
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx;
                int my;

                SDL_GetMouseState(&mx, &my);

                player.dst.x = mx - player.dst.w / 2;
                player.y_float = my - player.dst.h / 2;
                player.dst.y = (int)player.y_float;
            }
        }

        /********************************************/
        /*              Mise à jour                  */
        /********************************************/

        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

        update_player(&player, keyboard, &moving);
        update_attack(&player);
        update_animation(&player, moving, &last_frame_time);

        SDL_Rect attack_box = get_attack_box(player);

        if (player.attacking && !player.attack_already_hit) {
            if (SDL_HasIntersection(&attack_box, &enemy)) {
                enemy_life -= 20;

                if (enemy_life < 0) {
                    enemy_life = 0;
                }

                player.attack_already_hit = 1;
            }
        }

        scroll_x += 2;

        /********************************************/
        /*              Affichage                    */
        /********************************************/

        draw_background(renderer, background, scroll_x);
        draw_ground(renderer);

        draw_enemy(renderer, enemy_texture, enemy, enemy_life);

        if (player.attacking) {
            SDL_SetRenderDrawColor(renderer, 255, 220, 50, 120);
            SDL_RenderFillRect(renderer, &attack_box);
        }

        draw_player(renderer,
                    player_texture,
                    player,
                    frame_w,
                    frame_h);

        draw_life_bar(renderer, enemy, enemy_life);

        draw_text(renderer,
                  font,
                  "Fleches: bouger | Z: sauter | F/Espace: frapper | clic: teleporter | q: quitter",
                  20,
                  20);

        if (enemy_life == 0) {
            draw_text(renderer, font, "Ennemi KO !", 680, 60);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    clean(window, renderer, background, player_texture, enemy_texture, font);

    return EXIT_SUCCESS;
}