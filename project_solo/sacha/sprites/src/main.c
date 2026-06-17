#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <SDL2/SDL_image.h>

#define SPRITE_NB_W 6
#define SPRITE_NB_H 5
#define SPRITE_SHEET_PLAYER "assets/player.png"
#define GROUND_TEXTURE "assets/tile.png"

typedef struct{
    SDL_Rect hitbox, state;
    int current_frame;
    float speed;
    SDL_bool key_up, key_down, key_left, key_right;
    SDL_Texture * texture;
} Entity;

void end_sdl(
    int ok,
    char const* msg,
    SDL_Window* window,
    SDL_Renderer* renderer
) {
    char msg_formated[255];
    int l;

    if (!ok) {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");
        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();
    IMG_Quit();

    if (!ok) {
        exit(EXIT_FAILURE);
    }
}

SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer ){
    SDL_Surface *my_image = NULL;
    SDL_Texture* my_texture = NULL;

    my_image = IMG_Load(file_image_name);

    if (my_image == NULL) end_sdl(0, "Chargement de l'image impossible", window, renderer);
   
    my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);
    if (my_texture == NULL) end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

    return my_texture;
}

Entity * initPlayer(SDL_Window * window, SDL_Renderer * renderer, int window_w, int window_h){
    Entity * player=calloc(1, sizeof(Entity));
    int zoom = 2;
    float hitbox_scale = 0.5f;

    player->texture = load_texture_from_image(SPRITE_SHEET_PLAYER, window, renderer);
    player->current_frame = 0;
    player->speed = 10;

    SDL_QueryTexture(player->texture, NULL, NULL, &player->state.w, &player->state.h);

    int offset_x = player->state.w / SPRITE_NB_W;
    int offset_y = player->state.h / SPRITE_NB_H;

    player->state.x = 0;
    player->state.y = offset_y*2;
    player->state.w = offset_x;
    player->state.h = offset_y;

    player->hitbox.w = offset_x*zoom*hitbox_scale;
    player->hitbox.h = offset_y*zoom*hitbox_scale;
    
    player->hitbox.x =(window_w - player->hitbox.w) /2;
    player->hitbox.y =(window_h - player->hitbox.h) /2;

    player->key_up = SDL_FALSE;
    player->key_down = SDL_FALSE;
    player->key_left = SDL_FALSE;
    player->key_right = SDL_FALSE;

    return player;
}

void moveCamera(SDL_Rect * camera, SDL_Rect * window_rect, int direction_x, int direction_y){
    int next_camera_x = camera->x - direction_x;
    int next_camera_y = camera->y - direction_y;

    if (next_camera_x >= 0){
        next_camera_x = 0;
    }
    if (next_camera_x <= window_rect->w - camera->w){
        next_camera_x = window_rect->w - camera->w;
    }
    if (next_camera_y >= 0){
        next_camera_y = 0;
    }
    if (next_camera_y <= window_rect->h - camera->h){
        next_camera_y = window_rect->h - camera->h;
    }

    camera->x = next_camera_x;
    camera->y = next_camera_y;
}

int movePlayer(Entity * player, int screen_w, int screen_h, SDL_Rect * camera, SDL_Rect * world_rect){
    if (player == NULL){
        return 0;
    }

    int dirx = (player->key_right && !player->key_left) ? 1
                : (player->key_left  && !player->key_right) ? -1 : 0;
    int diry = (dirx != 0) ? 0
                : (player->key_down && !player->key_up)  ?  1
                : (player->key_up   && !player->key_down) ? -1 : 0;


    if (dirx != 0 || diry != 0){
        float new_x = player->hitbox.x + dirx * player->speed;
        float new_y = player->hitbox.y + diry * player->speed;

        if (new_x < 0) {
            new_x = 0;
            moveCamera(camera, world_rect, dirx * player->speed, 0);
        }
        if (new_y < 0) {
            new_y = 0;
            moveCamera(camera, world_rect, 0, diry * player->speed);
        }
        if (new_x > screen_w - player->hitbox.w) {
            new_x = screen_w - player->hitbox.w;
            moveCamera(camera, world_rect, dirx * player->speed, 0);
        }
        if (new_y > screen_h - player->hitbox.h) {
            new_y = screen_h - player->hitbox.h;
            moveCamera(camera, world_rect, 0, diry * player->speed);
        }

        player->hitbox.x = new_x;
        player->hitbox.y = new_y;

    }

    return 1;
}

void drawEntity(SDL_Renderer * renderer, Entity * entity, Uint32 * last_time){
    if (renderer == NULL || entity == NULL){
        return;
    }

    Uint32 current_time = SDL_GetTicks();
    Uint32 frame_delay = 100;

    if ((current_time - *last_time > frame_delay)){
        entity->current_frame = (entity->current_frame + 1) % SPRITE_NB_W;
        *last_time = current_time;
    }
    if(!(entity->key_right || entity->key_left || entity->key_up || entity->key_down)){
        entity->state.y = entity->state.h*4;
    }
    else if (entity->key_right && !entity->key_left){
        entity->state.y = entity->state.h*2;
    }else if (!entity->key_right && entity->key_left){
        entity->state.y = entity->state.h;
    }else if (entity->key_down && !entity->key_up){
        entity->state.y = 0;
    }else if (!entity->key_down && entity->key_up){
        entity->state.y = entity->state.h*3;
    }

    entity->state.x = entity->current_frame * entity->state.w;

    SDL_RenderCopy(renderer, entity->texture,
        &entity->state,
        &entity->hitbox);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &entity->hitbox);
}

int main(int argc, char **argv){
    (void)argc;
    (void)argv;

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    SDL_Event event;

    SDL_bool running = SDL_TRUE;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(0, "ERROR SDL INIT", window, renderer);
    }

    window = SDL_CreateWindow(
        "Fenêtre Principal",
        0, 0,
        0, 0,
        SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == NULL ) {
        end_sdl(0, "ERROR WINDOW INIT", window, renderer);
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
        end_sdl(0, "ERROR RENDERER INIT", window, renderer);
    }

    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);

    Entity * player=initPlayer(window, renderer, window_w, window_h);
    
    SDL_Texture * ground = load_texture_from_image(GROUND_TEXTURE, window, renderer);
    SDL_Rect rect_window = {0};
    rect_window.w = window_w;
    rect_window.h = window_h;
    SDL_Rect rect_camera = {0};
    SDL_QueryTexture(ground, NULL, NULL, &rect_camera.w, &rect_camera.h);
    rect_camera.x = (rect_window.w - rect_camera.w)/2;
    rect_camera.y = (rect_window.h - rect_camera.h)/2;

    Uint32 start_time = SDL_GetTicks();

    while (running){
        while(SDL_PollEvent(&event)){

            switch (event.type){
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = SDL_FALSE;
                            break;
                        case SDLK_RSHIFT:
                            player->speed *= (player->speed*1.5 >= 60) ? 1 : 1.5;
                            break;
                        case SDLK_LSHIFT:
                            player->speed /= (player->speed / 1.5 <= 4) ? 1 : 1.5;
                            break;
                        case SDLK_z:
                            player->key_up = SDL_TRUE;
                            break;
                        case SDLK_s:
                            player->key_down = SDL_TRUE;
                            break;
                        case SDLK_q:
                            player->key_left = SDL_TRUE;
                            break;
                        case SDLK_d:
                            player->key_right = SDL_TRUE;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_z:
                            player->key_up = SDL_FALSE;
                            break;
                        case SDLK_s:
                            player->key_down = SDL_FALSE;
                            break;
                        case SDLK_q:
                            player->key_left = SDL_FALSE;
                            break;
                        case SDLK_d:
                            player->key_right = SDL_FALSE;
                            break;
                        case SDLK_ESCAPE:
                            running = SDL_FALSE;
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        movePlayer(player, window_w, window_h, &rect_camera, &rect_window);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, ground, NULL, &rect_camera);

        drawEntity(renderer, player, &start_time);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_Delay(10);

    end_sdl(1, "Normal ending", window, renderer);
    SDL_DestroyTexture(player->texture);
    free(player);
    return 0;
}