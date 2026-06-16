#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600
#define VITESSE_DEFILEMENT 3

// Information du spritesheet
#define DEB_X 182
#define DEB_Y 200
#define TAILLE_IMG 227
#define NB_IMAGE 6

#define DELAI_ANIMATION 6 // Nombre d'itérations avant de changer de frame

#define SOLEIL_X 600
#define SOLEIL_Y 50
#define SOLEIL_LARGEUR 100
#define SOLEIL_HAUTEUR 100

typedef struct {
    SDL_Texture *texture;
    int w, h;
} Tex;

// Personnage
typedef struct {
    SDL_Texture *spritesheet;
    int frame_w, frame_h; // Dimensions d'une sous image
    int x, y;
    int orientation; // -1 gauche, 1 droite
    bool immobile; // true = arrêt, false = en mouvement
    int frame_index;
    int compteur_anim;
} Personnage;

static void end_sdl(bool ok, char const *msg,
                    SDL_Window *window, SDL_Renderer *renderer,
                    SDL_Texture *tex1, SDL_Texture *tex2, SDL_Texture *tex3) {
    if (tex1) SDL_DestroyTexture(tex1);
    if (tex2) SDL_DestroyTexture(tex2);
    if (tex3) SDL_DestroyTexture(tex3);
    if (!ok) SDL_Log("%s : %s\n", msg, SDL_GetError());
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    if (!ok) exit(EXIT_FAILURE);
}

static SDL_Texture* load_texture_from_image(const char *file_image_name,
                                            SDL_Window *window,
                                            SDL_Renderer *renderer) {
    SDL_Surface *my_image = IMG_Load(file_image_name);
    if (!my_image)
        end_sdl(false, "Chargement de l'image impossible", window, renderer, NULL, NULL, NULL);

    SDL_Texture *my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);
    if (!my_texture)
        end_sdl(false, "Echec de la transformation en texture", window, renderer, NULL, NULL, NULL);

    return my_texture;
}

void draw_texture_repeat(Tex t, int offset, SDL_Renderer *renderer, int y_pos) {
    int start = (t.w - (offset % t.w)) % t.w;
    for (int x = start - t.w; x < LARGEUR_FENETRE; x += t.w) {
        SDL_Rect dst = {x, y_pos, t.w, t.h};
        SDL_RenderCopy(renderer, t.texture, NULL, &dst);
    }
}

void init_personnage(Personnage *p, SDL_Window *window, SDL_Renderer *renderer) {
    p->spritesheet = load_texture_from_image("assets/spritesheet.png", window, renderer);

    p->frame_w = TAILLE_IMG;
    p->frame_h = TAILLE_IMG;

    p->x = (LARGEUR_FENETRE - p->frame_w) / 2;
    p->y = HAUTEUR_FENETRE - p->frame_h - 80;

    p->orientation = 1; // Regarde vers la droite au départ
    p->immobile = true; // Immobile au début
    p->frame_index = 0;
    p->compteur_anim = 0;
}

void update_personnage(Personnage *p) {
    if (!p->immobile) { // En mouvement
        p->compteur_anim++;
        if (p->compteur_anim >= DELAI_ANIMATION) {
            p->compteur_anim = 0;
            p->frame_index++; // Nouvelle frame
            if (p->frame_index >= NB_IMAGE) {
                p->frame_index = 1; // On revient à la première frame
            }
        }
    } else { // Immobile
        p->frame_index = 0;
        p->compteur_anim = 0;
    }
}

void draw_personnage(SDL_Renderer *renderer, const Personnage *p) {
    SDL_Rect src = {
        DEB_X + p->frame_index * TAILLE_IMG,
        DEB_Y,
        TAILLE_IMG,
        TAILLE_IMG
    };

    SDL_Rect dst = {
        p->x,
        p->y,
        p->frame_w,
        p->frame_h
    };

    SDL_RendererFlip flip = (p->orientation == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE; // Fait la symétrie de l'image si on va à gauche

    SDL_RenderCopyEx(renderer, p->spritesheet, &src, &dst, 0.0, NULL, flip);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        end_sdl(false, "SDL_Init", NULL, NULL, NULL, NULL, NULL);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        end_sdl(false, "IMG_Init", NULL, NULL, NULL, NULL, NULL);

    SDL_Window *window = SDL_CreateWindow("Sol défilant avec personnage",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          LARGEUR_FENETRE, HAUTEUR_FENETRE,
                                          SDL_WINDOW_SHOWN);
    if (!window)
        end_sdl(false, "SDL_CreateWindow", NULL, NULL, NULL, NULL, NULL);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        end_sdl(false, "SDL_CreateRenderer", window, NULL, NULL, NULL, NULL);

    Tex ground;
    ground.texture = load_texture_from_image("assets/ground.png", window, renderer);
    SDL_QueryTexture(ground.texture, NULL, NULL, &ground.w, &ground.h);
    int ground_y = HAUTEUR_FENETRE - ground.h;

    Tex sun;
    sun.texture = load_texture_from_image("assets/sun.png", window, renderer);
    SDL_QueryTexture(sun.texture, NULL, NULL, &sun.w, &sun.h);

    Personnage perso;
    init_personnage(&perso, window, renderer);

    int scroll_offset = 0;

    bool program_on = true;
    SDL_Event event;

    while (program_on) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    program_on = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            perso.orientation = -1;
                            perso.immobile = false;
                            break;
                        case SDLK_d:
                            perso.orientation = 1;
                            perso.immobile = false;
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            perso.immobile = true;
                            break;
                        case SDLK_d:
                            perso.immobile = true;
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        if (!perso.immobile) {
            scroll_offset += perso.orientation * VITESSE_DEFILEMENT; // Mise à jour du décalage du sol
        }

        update_personnage(&perso); // Mise à jour de l'animation du perso

        // Rendu
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // ciel
        SDL_RenderClear(renderer);

        SDL_Rect sun_dst = {SOLEIL_X, SOLEIL_Y, SOLEIL_LARGEUR, SOLEIL_HAUTEUR};
        SDL_RenderCopy(renderer, sun.texture, NULL, &sun_dst);

        draw_texture_repeat(ground, scroll_offset, renderer, ground_y); // Affiche le sol
        draw_personnage(renderer, &perso); // Affiche le perso

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    end_sdl(true, "Fin normale", window, renderer, ground.texture, sun.texture, perso.spritesheet);
    return EXIT_SUCCESS;
}