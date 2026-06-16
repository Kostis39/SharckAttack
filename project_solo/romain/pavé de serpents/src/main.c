#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define LARGEUR 1000
#define HAUTEUR 1000
#define NB_SEGMENTS 50
#define VITESSE 5.0f
#define ANGLE_VITESSE 0.1f
#define LONG_SEG 30.0f
#define LARG_SEG 15.0f

typedef struct {
    float x, y, angle;
} Position;

// File circulaire toujours pleine, taille fixe
typedef struct {
    Position *donnees;
    int capacite;
    int debut; // indice du segment le plus récent (tête du serpent)
} File;

typedef struct {
    File *file;
    float angle_tete;
    float angle_cible;
} Serpent;

// Alloue et remplit la file avec la position p (retourne un pointeur)
File *fileCreate(int capacite, Position p) {
    File *f = malloc(sizeof(File));
    if (!f) return NULL;
    f->donnees = malloc(capacite * sizeof(Position));
	if(!f->donnees) {
		free(f);
		return NULL;
	}
    f->capacite = capacite;
    f->debut = 0;
    if (f->donnees) {
        for (int i = 0; i < capacite; i++)
            f->donnees[i] = p;
    }
    return f;
}

void fileLiberer(File *f) {
	free(f->donnees);
	free(f);
}

// Enfile p en tête et écrase le plus ancien
void fileEnfiler(File *f, Position p) {
    f->debut = (f->debut - 1 + f->capacite) % f->capacite;
    f->donnees[f->debut] = p;
}

static void end_sdl(bool ok, char const *msg,
                    SDL_Window *window, SDL_Renderer *renderer) {
    if (!ok) SDL_Log("%s : %s\n", msg, SDL_GetError());
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    if (!ok) exit(EXIT_FAILURE);
}

void dessinerRectangle(SDL_Renderer *r, Position p) {
    float ca = cosf(p.angle), sa = sinf(p.angle);
    float lo = LONG_SEG/2, la = LARG_SEG/2;
    int x0 = p.x + lo*ca - la*sa, y0 = p.y + lo*sa + la*ca;
    int x1 = p.x + lo*ca + la*sa, y1 = p.y + lo*sa - la*ca;
    int x2 = p.x - lo*ca + la*sa, y2 = p.y - lo*sa - la*ca;
    int x3 = p.x - lo*ca - la*sa, y3 = p.y - lo*sa + la*ca;
    SDL_RenderDrawLine(r, x0, y0, x1, y1);
    SDL_RenderDrawLine(r, x1, y1, x2, y2);
    SDL_RenderDrawLine(r, x2, y2, x3, y3);
    SDL_RenderDrawLine(r, x3, y3, x0, y0);
}

void initialiserSerpent(Serpent *s) {
    s->angle_tete = 0;
    s->angle_cible = 0;
    Position depart = { LARGEUR/2, HAUTEUR/2, 0 };
    s->file = fileCreate(NB_SEGMENTS, depart);
}

float tournerVers(float angle, float cible, float vitesse) {
    float diff = fmodf(cible - angle, 2*M_PI); // Différence entre la cible et l'angle actuel
    if (diff > M_PI) diff -= 2*M_PI; // Pour rester dans l'intervalle [-pi;pi]
    if (diff < -M_PI) diff += 2*M_PI;
    if (fabsf(diff) <= vitesse) return cible; // On atteint l'angle souhaité
    return angle + (diff > 0 ? vitesse : -vitesse);
}

bool deplacerSerpent(Serpent *s, int souris_x, int souris_y) {
    Position tete = s->file->donnees[s->file->debut];

    float dx = souris_x - tete.x;
    float dy = souris_y - tete.y;
    s->angle_cible = atan2f(dy, dx);

    s->angle_tete = tournerVers(s->angle_tete, s->angle_cible, ANGLE_VITESSE);

    float nx = tete.x + cosf(s->angle_tete) * VITESSE;
    float ny = tete.y + sinf(s->angle_tete) * VITESSE;

    if (nx < 0 || nx > LARGEUR || ny < 0 || ny > HAUTEUR)
        return false;

    fileEnfiler(s->file, (Position){ nx, ny, s->angle_tete });
    return true;
}

// Parcourt la file et dessine chaque segment
void dessinerSerpent(SDL_Renderer *renderer, const Serpent *s) {
    for (int i = 0; i < s->file->capacite; i++) {
        Position p = s->file->donnees[(s->file->debut + i) % s->file->capacite];
        dessinerRectangle(renderer, p);
    }
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        end_sdl(false, "SDL_Init", window, renderer);

    window = SDL_CreateWindow("Serpent", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, LARGEUR, HAUTEUR,
                              SDL_WINDOW_SHOWN);
    if (!window) end_sdl(false, "SDL_CreateWindow", window, renderer);

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) end_sdl(false, "SDL_CreateRenderer", window, renderer);

    Serpent serpent;
    initialiserSerpent(&serpent);

    bool program_on = true;
    SDL_Event event;
    int souris_x = LARGEUR / 2, souris_y = HAUTEUR / 2;

    while (program_on) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: program_on = false; break;
            case SDL_MOUSEMOTION:
                souris_x = event.motion.x;
                souris_y = event.motion.y;
                break;
            default: break;
            }
        }

        if (!deplacerSerpent(&serpent, souris_x, souris_y)) // On sort de l'écran
            program_on = false;

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        dessinerSerpent(renderer, &serpent);
        SDL_RenderPresent(renderer);
    }

    fileLiberer(serpent.file);
    end_sdl(true, "Fin normale", window, renderer);
    return EXIT_SUCCESS;
}