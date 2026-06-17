#include "sprite.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>

typedef struct player {
  SDL_Rect pos;
  SDL_Rect src;
  int direction;
  int nbSprite;
  int currentFrame;
  int scale;
  int mapW;
  int mapH;
  SDL_Texture *imageTexture;
} player_t;

player_t *spawnPlayer(char *path, SDL_Renderer *ren, int mapW, int mapH,
                      int nbSprite, int scale) {
  player_t *p = malloc(sizeof(player_t));
  if (!p)
    exit(EXIT_FAILURE);

  p->imageTexture = load_texture_from_image(path, ren);
  if (!p->imageTexture)
    exit(EXIT_FAILURE);

  int texW, texH;
  SDL_QueryTexture(p->imageTexture, NULL, NULL, &texW, &texH);
  p->scale = scale;
  p->nbSprite = nbSprite;
  p->currentFrame = 0;
  p->src.w = texW / nbSprite;
  p->src.h = texH;
  p->src.x = 0;
  p->src.y = 0;

  p->mapW = mapW;
  p->mapH = mapH;
  p->pos.w = p->src.w * p->scale;
  p->pos.h = p->src.h * p->scale;
  p->pos.x = (mapW - p->pos.w) / 2;
  p->pos.y = (mapH - p->pos.h) / 2;
  p->direction = 0;

  return p;
}

player_t *animate(player_t *p) {
  p->currentFrame = (p->currentFrame + 1) % p->nbSprite;
  p->src.x = p->currentFrame * p->src.w;
  return p;
}

player_t *playerState(player_t *p, SDL_Texture *newImage, int frameCount,
                      int direction) {
  int texW, texH;
  SDL_QueryTexture(newImage, NULL, NULL, &texW, &texH);
  p->src.w = texW / frameCount;
  p->src.h = texH;
  p->src.x = 0;
  p->nbSprite = frameCount;
  p->currentFrame = 0;
  p->imageTexture = newImage;
  p->pos.w = p->src.w * p->scale;
  p->pos.h = p->src.h * p->scale;
  p->pos.x = (p->mapW - p->pos.w) / 2;
  p->pos.y = (p->mapH - p->pos.h) / 2;
  p->direction = direction;
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

  player_t *player =
      spawnPlayer("assets/robotSprite.png", ren, maxX, maxY, 12, 4);
  SDL_Texture *playerRunning =
      load_texture_from_image("assets/robotSprite_run.png", ren);
  SDL_Texture *playerIdle =
      load_texture_from_image("assets/robotSprite.png", ren);
  Uint32 lastAnimTime = SDL_GetTicks();
  const Uint32 animDelay = 80;
  int direction = 1;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          running = 0;
          break;

        case SDLK_LEFT:
          if (event.key.repeat == 0) {
            direction = -1;
            playerState(player, playerRunning, 12, direction);
          }
          break;

        case SDLK_RIGHT:
          if (event.key.repeat == 0) {
            direction = 1;
            playerState(player, playerRunning, 12, direction);
          }
          break;
        }
      } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LEFT ||
            event.key.keysym.sym == SDLK_RIGHT) {
          playerState(player, playerIdle, 12, direction);
        }
      }
    }

    Uint32 now = SDL_GetTicks();
    if (now - lastAnimTime >= animDelay) {
      animate(player);
      lastAnimTime = now;
    }

    SDL_RenderCopy(ren, background, &source,
                   &destination); // Création de l'élément à afficher
    SDL_RendererFlip flip =
        (player->direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(ren, player->imageTexture, &player->src, &player->pos, 0,
                     NULL, flip);
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
