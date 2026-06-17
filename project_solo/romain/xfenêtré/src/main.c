#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IMG_PATH "src/dvd_logo.png"
#define MAX_FENETRES 10

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *texture;
    int x, y;
    int vx, vy;
    int w, h;
} DVDWindow;

DVDWindow creer_fenetre(SDL_Surface *image_surface, int ecran_w, int ecran_h) {
    DVDWindow dw;

    dw.w = 400;
    dw.h = 300;

    dw.x = rand() % (ecran_w - dw.w);
    dw.y = rand() % (ecran_h - dw.h);

    dw.vx = rand() % 10 * (rand() % 2 ? 1 : -1); // Vitesse entre -10 et 10
    dw.vy = rand() % 10 * (rand() % 2 ? 1 : -1);


    dw.window = SDL_CreateWindow("DVD",
                                 dw.x, dw.y,
                                 dw.w, dw.h,
                                 SDL_WINDOW_RESIZABLE);
    if (!dw.window) {
        SDL_Log("Erreur création fenêtre : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    dw.renderer = SDL_CreateRenderer(dw.window, -1, SDL_RENDERER_ACCELERATED);
    if (!dw.renderer) {
        SDL_Log("Erreur création renderer : %s", SDL_GetError());
        SDL_DestroyWindow(dw.window);
        exit(EXIT_FAILURE);
    }

    dw.texture = SDL_CreateTextureFromSurface(dw.renderer, image_surface);
    if (!dw.texture) {
        SDL_Log("Erreur création texture : %s", SDL_GetError());
        SDL_DestroyRenderer(dw.renderer);
        SDL_DestroyWindow(dw.window);
        exit(EXIT_FAILURE);
    }

    return dw;
}

void detruire_fenetre(DVDWindow *dw) {
    SDL_DestroyTexture(dw->texture);
    SDL_DestroyRenderer(dw->renderer);
    SDL_DestroyWindow(dw->window);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("Erreur IMG_Init : %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int ecran_w = DM.w;
    int ecran_h = DM.h - 44; // Pour prendre en compte de la barre des tâches

    SDL_Surface *image_surface = IMG_Load(IMG_PATH);
    if (!image_surface) {
        SDL_Log("Erreur chargement image : %s", IMG_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    DVDWindow fenetres[MAX_FENETRES];
    fenetres[0] = creer_fenetre(image_surface, ecran_w, ecran_h);
    int nb_fenetres = 1;

    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;
    SDL_bool pause = SDL_FALSE;

    while (program_on) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            pause = !pause;
                            break;

                        case SDLK_ESCAPE:
                            program_on = SDL_FALSE;
                            break;

                        case SDLK_RETURN: // Ajouter une fenêtre
                            if (nb_fenetres < MAX_FENETRES) {
                                fenetres[nb_fenetres] = creer_fenetre(image_surface, ecran_w, ecran_h);
                                nb_fenetres++;
                            }
                            break;

                        case SDLK_BACKSPACE: // Supprimer la dernière fenêtre
                            if (nb_fenetres > 1) {
                                nb_fenetres--;
                                detruire_fenetre(&fenetres[nb_fenetres]);
                            }
                            break;

                        case SDLK_RIGHT:
                            for (int i = 0; i < nb_fenetres; i++) {
                                if (fenetres[i].vx >= 0) fenetres[i].vx++;
                                else fenetres[i].vx--;
                            }
                            break;
                        case SDLK_LEFT:
                            for (int i = 0; i < nb_fenetres; i++) {
                                if (fenetres[i].vx > 0) fenetres[i].vx--;
                                else if (fenetres[i].vx < 0) fenetres[i].vx++;
                            }
                            break;
                        case SDLK_UP:
                            for (int i = 0; i < nb_fenetres; i++) {
                                if (fenetres[i].vy >= 0) fenetres[i].vy++;
                                else fenetres[i].vy--;
                            }
                            break;
                        case SDLK_DOWN:
                            for (int i = 0; i < nb_fenetres; i++) {
                                if (fenetres[i].vy > 0) fenetres[i].vy--;
                                else if (fenetres[i].vy < 0) fenetres[i].vy++;
                            }
                            break;
                    }
                    break;

                
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) { // Gestion du redimensionnement d'une fenêtre
                        for (int i = 0; i < nb_fenetres; i++) {
                            if (event.window.windowID == SDL_GetWindowID(fenetres[i].window)) {
                                SDL_GetWindowSize(fenetres[i].window, &fenetres[i].w, &fenetres[i].h);
                                break;
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (!pause) {
            for (int i = 0; i < nb_fenetres; i++) {
                DVDWindow *dw = &fenetres[i];

                // Nouvelle position
                int new_x = dw->x + dw->vx;
                int new_y = dw->y + dw->vy;

                // Rebonds sur les bords de l'écran
                if (new_x <= 0) {
                    new_x = 0;
                    dw->vx = -dw->vx;
                } else if (new_x + dw->w >= ecran_w) {
                    new_x = ecran_w - dw->w;
                    dw->vx = -dw->vx;
                }

                if (new_y <= 0) {
                    new_y = 0;
                    dw->vy = -dw->vy;
                } else if (new_y + dw->h >= ecran_h) {
                    new_y = ecran_h - dw->h;
                    dw->vy = -dw->vy;
                }

                SDL_SetWindowPosition(dw->window, new_x, new_y);
                dw->x = new_x;
                dw->y = new_y;

                SDL_RenderClear(dw->renderer);
                SDL_RenderCopy(dw->renderer, dw->texture, NULL, NULL);
                SDL_RenderPresent(dw->renderer);
            }
        }

        SDL_Delay(10);
    }

    for (int i = 0; i < nb_fenetres; i++) {
        detruire_fenetre(&fenetres[i]);
    }
    SDL_FreeSurface(image_surface);
    IMG_Quit();
    SDL_Quit();

    return 0;
}