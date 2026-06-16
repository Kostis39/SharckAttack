#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 650

#define SPRITE_NB_IMAGES 8
#define SPRITE_NB_LINES 4

#define PLAYER_SPEED 6
#define ANIMATION_DELAY 80

void end_sdl(char ok,
             char const *msg,
             SDL_Window *window,
             SDL_Renderer *renderer) {
    char msg_formated[255];
    int l;

    if (!ok) {
        strncpy(msg_formated, msg, 250);
        msg_formated[250] = '\0';
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

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    if (!ok) {
        exit(EXIT_FAILURE);
    }
}

SDL_Texture *load_texture_from_image(char *file_image_name,
                                     SDL_Window *window,
                                     SDL_Renderer *renderer) {
    SDL_Surface *my_image = NULL;
    SDL_Texture *my_texture = NULL;

    my_image = IMG_Load(file_image_name);

    if (my_image == NULL) {
        end_sdl(0, "Chargement de l'image impossible", window, renderer);
    }

    my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);

    if (my_texture == NULL) {
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);
    }

    return my_texture;
}

SDL_Texture *create_text_texture(SDL_Renderer *renderer,
                                 TTF_Font *font,
                                 char *text,
                                 SDL_Color color,
                                 SDL_Window *window) {
    SDL_Surface *text_surface = NULL;
    SDL_Texture *text_texture = NULL;

    text_surface = TTF_RenderUTF8_Blended(font, text, color);

    if (text_surface == NULL) {
        end_sdl(0, "Impossible de creer la surface du texte", window, renderer);
    }

    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);

    if (text_texture == NULL) {
        end_sdl(0, "Impossible de creer la texture du texte", window, renderer);
    }

    return text_texture;
}

void draw_background(SDL_Texture *background_texture,
                     SDL_Window *window,
                     SDL_Renderer *renderer,
                     int scroll_x) {
    SDL_Rect source = {0};
    SDL_Rect window_dimensions = {0};
    SDL_Rect destination = {0};

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);

    SDL_QueryTexture(background_texture, NULL, NULL, &source.w, &source.h);

    if (source.w > window_dimensions.w) {
        source.x = scroll_x % (source.w - window_dimensions.w);
        source.w = window_dimensions.w;
    } else {
        source.x = 0;
    }

    if (source.h > window_dimensions.h) {
        source.y = 200;
        source.h = window_dimensions.h;
    } else {
        source.y = 0;
    }

    destination = window_dimensions;

    SDL_RenderCopy(renderer, background_texture, &source, &destination);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Texture *background_texture = NULL;
    SDL_Texture *sprite_texture = NULL;
    SDL_Texture *text_texture = NULL;

    TTF_Font *font = NULL;

    SDL_DisplayMode screen;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(0, "ERROR SDL INIT", window, renderer);
    }

    if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)) == 0) {
        end_sdl(0, "ERROR IMG INIT", window, renderer);
    }

    if (TTF_Init() < 0) {
        end_sdl(0, "ERROR TTF INIT", window, renderer);
    }

    SDL_GetCurrentDisplayMode(0, &screen);

    window = SDL_CreateWindow("Animer des sprites",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        end_sdl(0, "ERROR WINDOW CREATION", window, renderer);
    }

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        end_sdl(0, "ERROR RENDERER CREATION", window, renderer);
    }

    background_texture = load_texture_from_image("assets/backgrounds/background.jpg",
                                                 window,
                                                 renderer);

    sprite_texture = load_texture_from_image("assets/sprites/player-spritemap-v9.png",
                                             window,
                                             renderer);

    font = TTF_OpenFont("assets/fonts/Pacifico.ttf", 28);

    if (font == NULL) {
        end_sdl(0, "Impossible de charger la police", window, renderer);
    }

    SDL_Color text_color = {255, 255, 255, 255};

    text_texture = create_text_texture(renderer,
                                       font,
                                       "Fleches : deplacer | Clic gauche : teleporter | q/Echap : quitter",
                                       text_color,
                                       window);

    SDL_Rect sprite_source = {0};
    SDL_Rect sprite_state = {0};
    SDL_Rect sprite_destination = {0};
    SDL_Rect text_position = {20, 15, 0, 0};

    SDL_QueryTexture(sprite_texture, NULL, NULL, &sprite_source.w, &sprite_source.h);

    int offset_x = sprite_source.w / SPRITE_NB_IMAGES;
    int offset_y = sprite_source.h / SPRITE_NB_LINES;

    sprite_state.x = 0;
    sprite_state.y = 3 * offset_y;
    sprite_state.w = offset_x;
    sprite_state.h = offset_y;

    float zoom = 2.5;

    sprite_destination.w = offset_x * zoom;
    sprite_destination.h = offset_y * zoom;
    sprite_destination.x = 100;
    sprite_destination.y = WINDOW_HEIGHT - sprite_destination.h - 60;

    SDL_QueryTexture(text_texture, NULL, NULL, &text_position.w, &text_position.h);

    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;

    int current_frame = 0;
    int scroll_x = 0;

    Uint32 last_animation_time = SDL_GetTicks();

    while (program_on) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                program_on = SDL_FALSE;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    program_on = SDL_FALSE;
                    break;

                default:
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                    int mouse_x;
                    int mouse_y;

                    SDL_GetMouseState(&mouse_x, &mouse_y);

                    sprite_destination.x = mouse_x - sprite_destination.w / 2;
                    sprite_destination.y = mouse_y - sprite_destination.h / 2;
                }
                break;

            default:
                break;
            }
        }

        const Uint8 *keystates = SDL_GetKeyboardState(NULL);

        if (keystates[SDL_SCANCODE_LEFT]) {
            sprite_destination.x -= PLAYER_SPEED;
        }

        if (keystates[SDL_SCANCODE_RIGHT]) {
            sprite_destination.x += PLAYER_SPEED;
        }

        if (keystates[SDL_SCANCODE_UP]) {
            sprite_destination.y -= PLAYER_SPEED;
        }

        if (keystates[SDL_SCANCODE_DOWN]) {
            sprite_destination.y += PLAYER_SPEED;
        }

        if (sprite_destination.x < 0) {
            sprite_destination.x = 0;
        }

        if (sprite_destination.y < 0) {
            sprite_destination.y = 0;
        }

        if (sprite_destination.x + sprite_destination.w > WINDOW_WIDTH) {
            sprite_destination.x = WINDOW_WIDTH - sprite_destination.w;
        }

        if (sprite_destination.y + sprite_destination.h > WINDOW_HEIGHT) {
            sprite_destination.y = WINDOW_HEIGHT - sprite_destination.h;
        }

        Uint32 now = SDL_GetTicks();

        if (now - last_animation_time > ANIMATION_DELAY) {
            current_frame = (current_frame + 1) % SPRITE_NB_IMAGES;
            sprite_state.x = current_frame * offset_x;
            last_animation_time = now;
        }

        scroll_x += 1;

        SDL_RenderClear(renderer);

        draw_background(background_texture, window, renderer, scroll_x);

        SDL_RenderCopy(renderer,
                       sprite_texture,
                       &sprite_state,
                       &sprite_destination);

        SDL_RenderCopy(renderer,
                       text_texture,
                       NULL,
                       &text_position);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(sprite_texture);
    SDL_DestroyTexture(background_texture);

    TTF_CloseFont(font);

    end_sdl(1, "Normal ending", window, renderer);

    return EXIT_SUCCESS;
}
