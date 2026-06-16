#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define LARGEUR        800
#define HAUTEUR        600
#define NB_SEGMENTS    30
#define VITESSE        4.0f
#define ANGLE_VITESSE  0.12f
#define LONG_SEG       30.0f
#define LARG_SEG       15.0f

typedef struct {
	float x, y, angle;
} Position;

// File circulaire toujours pleine, taille fixe
typedef struct {
	Position *donnees;
	int       capacite;
	int       debut;   // indice du segment le plus récent (tête du serpent)
} File;

typedef struct {
	File  file;
	float angle_tete;
	float angle_cible;
} Serpent;

// Alloue et remplit la file avec la position p
File fileCreate(int capacite, Position p) {
	File f = { malloc(capacite * sizeof(Position)), capacite, 0 };
	if (f.donnees)
		for (int i = 0; i < capacite; i++)
			f.donnees[i] = p;
	return f;
}

void fileLiberer(File *f) {
	free(f->donnees);
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
	if (window)   SDL_DestroyWindow(window);
	SDL_Quit();
	if (!ok) exit(EXIT_FAILURE);
}

void dessinerRectangle(SDL_Renderer *r, Position p) {
	float ca = cosf(p.angle), sa = sinf(p.angle);
	float hl = LONG_SEG / 2.0f, hw = LARG_SEG / 2.0f;
	int x0 = (int)(p.x + hl*ca - hw*sa), y0 = (int)(p.y + hl*sa + hw*ca);
	int x1 = (int)(p.x + hl*ca + hw*sa), y1 = (int)(p.y + hl*sa - hw*ca);
	int x2 = (int)(p.x - hl*ca + hw*sa), y2 = (int)(p.y - hl*sa - hw*ca);
	int x3 = (int)(p.x - hl*ca - hw*sa), y3 = (int)(p.y - hl*sa + hw*ca);
	SDL_RenderDrawLine(r, x0, y0, x1, y1);
	SDL_RenderDrawLine(r, x1, y1, x2, y2);
	SDL_RenderDrawLine(r, x2, y2, x3, y3);
	SDL_RenderDrawLine(r, x3, y3, x0, y0);
}

// Parcourt la file et dessine chaque segment
void dessinerSerpent(SDL_Renderer *renderer, const Serpent *s) {
	File *f = &s->file;
	for (int i = 0; i < f->capacite; i++)
		dessinerRectangle(renderer, f->donnees[(f->debut + i) % f->capacite]);
}

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	return EXIT_SUCCESS;
}