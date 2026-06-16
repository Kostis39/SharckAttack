#ifndef WINDOW_TOOLS_H
#define WINDOW_TOOLS_H

#include <SDL2/SDL.h>

int get_display_size(int *width, int *height);
void print_display_mode(void);
void print_window_info(SDL_Window *window);
void move_window(SDL_Window *window, int dx, int dy);
void center_window(SDL_Window *window);

#endif
