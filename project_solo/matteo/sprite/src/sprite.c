#include "sprite.h"
#include <stdlib.h>

SDL_Texture *load_texture_from_image(char *file_image_name,
                                     SDL_Renderer *renderer) {
 SDL_Surface *my_image = NULL;   // Variable de passage
 SDL_Texture *my_texture = NULL; // La texture

 my_image =
     IMG_Load(file_image_name); // Chargement de l'image dans la surface
                                // image=SDL_LoadBMP(file_image_name);
                                // fonction standard de la SDL, uniquement
                                // possible si l'image est au format bmp */
 if (my_image == NULL)
  exit(1);

 my_texture = SDL_CreateTextureFromSurface(
     renderer,
     my_image); // Chargement de l'image de la surface vers la texture
 SDL_FreeSurface(
     my_image); // la SDL_Surface ne sert que comme élément transitoire
 if (my_texture == NULL)
  exit(1);
 return my_texture;
}
