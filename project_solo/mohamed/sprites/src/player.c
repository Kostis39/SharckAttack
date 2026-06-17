#include "player.h"

void init_player(Player *player,
                 int frame_w,
                 int frame_h) {
    player->dst.w = frame_w * 3;
    player->dst.h = frame_h * 3;

    player->dst.x = 120;
    player->dst.y = WINDOW_HEIGHT - GROUND_HEIGHT - player->dst.h;

    player->y_float = player->dst.y;
    player->velocity_y = 0.0f;

    player->facing_right = 1;
    player->grounded = 1;

    player->current_frame = 0;
    player->current_row = 0;

    player->attacking = SDL_FALSE;
    player->attack_start_time = 0;
    player->attack_already_hit = 0;
}

void start_jump(Player *player) {
    if (player->grounded) {
        player->velocity_y = JUMP_SPEED;
        player->grounded = 0;
    }
}

void start_attack(Player *player) {
    if (!player->attacking) {
        player->attacking = SDL_TRUE;
        player->attack_start_time = SDL_GetTicks();
        player->attack_already_hit = 0;

        player->current_row = 0;
        player->current_frame = 2;
    }
}

void update_player(Player *player,
                   const Uint8 *keyboard,
                   int *moving) {
    *moving = 0;

    if (!player->attacking) {
        if (keyboard[SDL_SCANCODE_LEFT]) {
            player->dst.x -= PLAYER_SPEED;
            player->facing_right = 0;
            *moving = 1;
        }

        if (keyboard[SDL_SCANCODE_RIGHT]) {
            player->dst.x += PLAYER_SPEED;
            player->facing_right = 1;
            *moving = 1;
        }
    }

    if (player->dst.x < 0) {
        player->dst.x = 0;
    }

    if (player->dst.x + player->dst.w > WINDOW_WIDTH) {
        player->dst.x = WINDOW_WIDTH - player->dst.w;
    }

    player->velocity_y += GRAVITY;
    player->y_float += player->velocity_y;
    player->dst.y = (int)player->y_float;

    int ground_y = WINDOW_HEIGHT - GROUND_HEIGHT - player->dst.h;

    if (player->dst.y >= ground_y) {
        player->dst.y = ground_y;
        player->y_float = player->dst.y;
        player->velocity_y = 0.0f;
        player->grounded = 1;
    }

    if (player->dst.y < 0) {
        player->dst.y = 0;
        player->y_float = player->dst.y;
        player->velocity_y = 0.0f;
    }
}

void update_attack(Player *player) {
    Uint32 now = SDL_GetTicks();

    if (player->attacking &&
        now - player->attack_start_time > ATTACK_DURATION) {
        player->attacking = SDL_FALSE;
    }
}

void update_animation(Player *player,
                      int moving,
                      Uint32 *last_frame_time) {
    Uint32 now = SDL_GetTicks();

    if (now - *last_frame_time > FRAME_DELAY) {
        if (player->attacking) {
            player->current_row = 0;
            player->current_frame++;

            if (player->current_frame > 5) {
                player->current_frame = 2;
            }
        } else if (!player->grounded) {
            /*
                Pendant le saut, on force une vignette visible.
                Cela évite que le joueur disparaisse si une frame est vide.
            */
            player->current_row = 0;
            player->current_frame = 0;
        } else if (moving) {
            player->current_row = 3;
            player->current_frame =
                (player->current_frame + 1) % SPRITE_COLUMNS;
        } else {
            player->current_row = 0;
            player->current_frame = 0;
        }

        *last_frame_time = now;
    }
}

SDL_Rect get_attack_box(Player player) {
    SDL_Rect box;

    box.w = 60;
    box.h = 60;
    box.y = player.dst.y + 55;

    if (player.facing_right) {
        box.x = player.dst.x + player.dst.w - 15;
    } else {
        box.x = player.dst.x - box.w + 15;
    }

    return box;
}

void draw_player(SDL_Renderer *renderer,
                 SDL_Texture *texture,
                 Player player,
                 int frame_w,
                 int frame_h) {
    if (texture == NULL) {
        SDL_SetRenderDrawColor(renderer, 40, 120, 240, 255);
        SDL_RenderFillRect(renderer, &player.dst);
        return;
    }

    SDL_Rect src;

    src.x = player.current_frame * frame_w;
    src.y = player.current_row * frame_h;
    src.w = frame_w;
    src.h = frame_h;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (!player.facing_right) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(renderer,
                     texture,
                     &src,
                     &player.dst,
                     0,
                     NULL,
                     flip);
}