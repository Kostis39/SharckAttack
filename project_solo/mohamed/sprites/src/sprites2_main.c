#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

/* Dimensions */
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define GROUND_HEIGHT 100

/* Joueur */
#define PLAYER_SPEED 6
#define JUMP_SPEED -11.0f
#define GRAVITY 0.55f

/* Sprite */
#define SPRITE_COLUMNS 8
#define SPRITE_ROWS 4
#define FRAME_DELAY 90

/* Combat */
#define ATTACK_DURATION 250

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

static void clean(SDL_Window *window,
                  SDL_Renderer *renderer,
                  SDL_Texture *background,
                  SDL_Texture *player,
                  SDL_Texture *enemy,
                  TTF_Font *font) {
    if (font != NULL) {
        TTF_CloseFont(font);
    }

    if (enemy != NULL) {
        SDL_DestroyTexture(enemy);
    }

    if (player != NULL) {
        SDL_DestroyTexture(player);
    }

    if (background != NULL) {
        SDL_DestroyTexture(background);
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

static SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);

    if (texture == NULL) {
        printf("Image non chargee : %s\n", path);
        printf("Erreur : %s\n", IMG_GetError());
        return NULL;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    return texture;
}

static void draw_text(SDL_Renderer *renderer,
                      TTF_Font *font,
                      const char *text,
                      int x,
                      int y) {
    if (font == NULL) {
        return;
    }

    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);

    if (surface == NULL) {
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == NULL) {
        return;
    }

    SDL_Rect dst = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
}

static void draw_background(SDL_Renderer *renderer,
                            SDL_Texture *background,
                            int scroll_x) {
    if (background != NULL) {
        SDL_Rect src = {0};
        SDL_Rect dst = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

        SDL_QueryTexture(background, NULL, NULL, &src.w, &src.h);

        if (src.w > WINDOW_WIDTH) {
            src.x = scroll_x % (src.w - WINDOW_WIDTH);
            src.w = WINDOW_WIDTH;
        }

        if (src.h > WINDOW_HEIGHT) {
            src.y = 100;
            src.h = WINDOW_HEIGHT;
        }

        SDL_RenderCopy(renderer, background, &src, &dst);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_RenderFillRect(renderer, &dst);
    } else {
        SDL_SetRenderDrawColor(renderer, 20, 25, 45, 255);
        SDL_RenderClear(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);

    for (int i = 0; i < 35; i++) {
        int x = ((i * 91) - scroll_x / 3) % WINDOW_WIDTH;
        int y = 40 + (i * 47) % 260;

        if (x < 0) {
            x += WINDOW_WIDTH;
        }

        SDL_RenderDrawPoint(renderer, x, y);
    }

    SDL_SetRenderDrawColor(renderer, 240, 220, 120, 255);
    SDL_Rect moon = {WINDOW_WIDTH - 120, 60, 50, 50};
    SDL_RenderFillRect(renderer, &moon);
}

static void draw_ground(SDL_Renderer *renderer) {
    SDL_Rect ground = {0, WINDOW_HEIGHT - GROUND_HEIGHT, WINDOW_WIDTH, GROUND_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 45, 45, 55, 255);
    SDL_RenderFillRect(renderer, &ground);

    SDL_Rect line = {0, WINDOW_HEIGHT - GROUND_HEIGHT, WINDOW_WIDTH, 8};
    SDL_SetRenderDrawColor(renderer, 130, 130, 150, 255);
    SDL_RenderFillRect(renderer, &line);

    SDL_SetRenderDrawColor(renderer, 70, 70, 85, 255);

    for (int x = 0; x < WINDOW_WIDTH; x += 80) {
        SDL_Rect brick = {x, WINDOW_HEIGHT - GROUND_HEIGHT + 25, 50, 10};
        SDL_RenderFillRect(renderer, &brick);
    }
}

static void init_player(Player *p, int frame_w, int frame_h) {
    p->dst.w = frame_w * 3;
    p->dst.h = frame_h * 3;
    p->dst.x = 120;
    p->dst.y = WINDOW_HEIGHT - GROUND_HEIGHT - p->dst.h;

    p->y_float = p->dst.y;
    p->velocity_y = 0.0f;

    p->facing_right = 1;
    p->grounded = 1;

    p->current_frame = 0;
    p->current_row = 0;

    p->attacking = SDL_FALSE;
    p->attack_start_time = 0;
    p->attack_already_hit = 0;
}

static void start_jump(Player *p) {
    if (p->grounded) {
        p->velocity_y = JUMP_SPEED;
        p->grounded = 0;
    }
}

static void start_attack(Player *p) {
    if (!p->attacking) {
        p->attacking = SDL_TRUE;
        p->attack_start_time = SDL_GetTicks();
        p->attack_already_hit = 0;
        p->current_row = 0;
        p->current_frame = 2;
    }
}

static void update_player(Player *p, const Uint8 *keyboard, int *moving) {
    *moving = 0;

    if (!p->attacking) {
        if (keyboard[SDL_SCANCODE_LEFT]) {
            p->dst.x -= PLAYER_SPEED;
            p->facing_right = 0;
            *moving = 1;
        }

        if (keyboard[SDL_SCANCODE_RIGHT]) {
            p->dst.x += PLAYER_SPEED;
            p->facing_right = 1;
            *moving = 1;
        }
    }

    if (p->dst.x < 0) {
        p->dst.x = 0;
    }

    if (p->dst.x + p->dst.w > WINDOW_WIDTH) {
        p->dst.x = WINDOW_WIDTH - p->dst.w;
    }

    p->velocity_y += GRAVITY;
    p->y_float += p->velocity_y;
    p->dst.y = (int)p->y_float;

    int ground_y = WINDOW_HEIGHT - GROUND_HEIGHT - p->dst.h;

    if (p->dst.y >= ground_y) {
        p->dst.y = ground_y;
        p->y_float = p->dst.y;
        p->velocity_y = 0.0f;
        p->grounded = 1;
    }
}

static void update_attack(Player *p) {
    Uint32 now = SDL_GetTicks();

    if (p->attacking && now - p->attack_start_time > ATTACK_DURATION) {
        p->attacking = SDL_FALSE;
    }
}

static void update_animation(Player *p, int moving, Uint32 *last_frame_time) {
    Uint32 now = SDL_GetTicks();

    if (now - *last_frame_time > FRAME_DELAY) {
        if (p->attacking) {
            p->current_row = 0;
            p->current_frame++;

            if (p->current_frame > 5) {
                p->current_frame = 2;
            }
        } else if (!p->grounded) {
            p->current_row = 0;
            p->current_frame = 0;
        } else if (moving) {
            p->current_row = 3;
            p->current_frame = (p->current_frame + 1) % SPRITE_COLUMNS;
        } else {
            p->current_row = 0;
            p->current_frame = 0;
        }

        *last_frame_time = now;
    }
}

static SDL_Rect get_attack_box(Player p) {
    SDL_Rect box;

    box.w = 60;
    box.h = 60;
    box.y = p.dst.y + 55;

    if (p.facing_right) {
        box.x = p.dst.x + p.dst.w - 15;
    } else {
        box.x = p.dst.x - box.w + 15;
    }

    return box;
}

static void draw_player(SDL_Renderer *renderer,
                        SDL_Texture *texture,
                        Player p,
                        int frame_w,
                        int frame_h) {
    if (texture == NULL) {
        SDL_SetRenderDrawColor(renderer, 40, 120, 240, 255);
        SDL_RenderFillRect(renderer, &p.dst);
        return;
    }

    SDL_Rect src;
    src.x = p.current_frame * frame_w;
    src.y = p.current_row * frame_h;
    src.w = frame_w;
    src.h = frame_h;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (!p.facing_right) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(renderer, texture, &src, &p.dst, 0, NULL, flip);
}

static void draw_enemy(SDL_Renderer *renderer,
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

static void draw_life_bar(SDL_Renderer *renderer, SDL_Rect enemy, int life_value) {
    SDL_Rect border = {enemy.x - 10, enemy.y - 25, 120, 12};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &border);

    SDL_Rect life = {enemy.x - 10, enemy.y - 25, life_value, 12};

    SDL_SetRenderDrawColor(renderer, 0, 220, 80, 255);
    SDL_RenderFillRect(renderer, &life);
}

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

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

    window = SDL_CreateWindow("Animer des sprites - combat simple",
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
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        printf("Erreur renderer : %s\n", SDL_GetError());
        clean(window, renderer, background, player_texture, enemy_texture, font);
        return EXIT_FAILURE;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    background = load_texture(renderer, "assets/backgrounds/background.jpg");
    player_texture = load_texture(renderer, "assets/sprites/player-spritemap-v9.png");
    enemy_texture = load_texture(renderer, "assets/sprites/enemy.png");

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

        scroll_x++;

        draw_background(renderer, background, scroll_x);
        draw_ground(renderer);

        draw_enemy(renderer, enemy_texture, enemy, enemy_life);

        if (player.attacking) {
            SDL_SetRenderDrawColor(renderer, 255, 220, 50, 120);
            SDL_RenderFillRect(renderer, &attack_box);
        }

        draw_player(renderer, player_texture, player, frame_w, frame_h);
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