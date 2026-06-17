#include "sprite.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>

typedef struct player {
  SDL_Rect pos;
  int direction;
  SDL_Texture *imageTexture;

} player_t;

player_t *spawnPlayer(char *path, SDL_Renderer *ren, int mapW, int mapH) {
  player_t *p = malloc(sizeof(player_t));
  if (!p)
    exit(EXIT_FAILURE);

  p->imageTexture = load_texture_from_image(path, ren);
  if (!p->imageTexture)
    exit(EXIT_FAILURE);

  int texW, texH;
  SDL_QueryTexture(p->imageTexture, NULL, NULL, &texW, &texH);

  p->pos.w = texW;
  p->pos.h = texH;
  p->pos.x = (mapW - texW) / 2;
  p->pos.y = (mapH - texH) / 2;
  p->direction = 0;

  return p;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  // Exécution de Xwindow

  SDL_Event event;
  int maxX;
  int maxY;
  int running = 1;
  /* Initialisation de la SDL  + gestion de l'échec possible */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n",
            SDL_GetError()); // l'initialisation de la SDL a échoué
    exit(EXIT_FAILURE);
  }

  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);

  maxY = dm.h;
  maxX = dm.w;

  SDL_Rect source = {0, 0, 0, 0};
  SDL_Rect destination = {0, 0, maxX, maxY};
  destination.h *= 1.2f;
  destination.w *= 1.2f;
  SDL_Window *window =
      SDL_CreateWindow("world", 0, 0, maxX, maxY, SDL_WINDOW_BORDERLESS);
  SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *background =
      load_texture_from_image("assets/background.png", ren);
  if (background != NULL) {
    SDL_QueryTexture(background, NULL, NULL, &source.w, &source.h);
  }

  player_t *player = spawnPlayer("assets/robotSprite.png", ren, maxX, maxY);

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          running = 0;
          break;
        }
      }
    }
    SDL_RenderCopy(ren, background, &source,
                   &destination); // Création de l'élément à afficher
    SDL_RenderCopy(ren, player->imageTexture, NULL, &player->pos);
    SDL_RenderPresent(ren);
  }
  SDL_DestroyTexture(player->imageTexture);
  free(player);
  IMG_Quit();
  SDL_DestroyTexture(background);
  SDL_DestroyRenderer(ren);
  SDL_Quit();
  return 0;
}
