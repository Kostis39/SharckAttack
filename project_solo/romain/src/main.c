#include <SDL2/SDL.h>
#include <stdio.h>

/************************************/
/*  exemple de création de fenêtres */
/************************************/

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;

    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n",
        SDL_GetError());   // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);
    }

    /* Création de la fenêtre */
    window = SDL_CreateWindow(
    "Fenêtre",                             // codage en utf8, donc accents possibles
    0, 0,                                  // coin haut gauche en haut gauche de l'écran
    400, 300,                              // largeur = 400, hauteur = 300
    SDL_WINDOW_RESIZABLE);                 // redimensionnable

    if (window == NULL) {
    SDL_Log("Error : SDL window 1 creation - %s\n", 
    SDL_GetError());                       // échec de la création de la fenêtre
    SDL_Quit();                            // On referme la SDL       
    exit(EXIT_FAILURE);
    }

    SDL_bool program_on = SDL_TRUE;               // Booléen pour dire que le programme doit continuer
    SDL_Event event;                              // c'est le type IMPORTANT !!
    SDL_bool pause = SDL_FALSE;

    int x = 100, y = 100;
    int vx = 3, vy = 3;
    int w, h;
    int ecran_w, ecran_h;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    ecran_w = DM.w;
    ecran_h = DM.h;

    ecran_h -= 44;
    
    SDL_GetWindowSize(window, &w, &h);

    while (program_on) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;

                case SDL_KEYDOWN: // Si une touche est pressée
                    if (event.key.keysym.sym == SDLK_SPACE) {  // Si c'est la touche Espace
                        pause = !pause;             // Mettre ou enlever la pause
                    } else if (event.key.keysym.sym == SDLK_RIGHT) {
                        vx++;
                    } else if (event.key.keysym.sym == SDLK_LEFT) {
                        vx--;
                    } else if (event.key.keysym.sym == SDLK_UP) {
                        vy--;
                    } else if (event.key.keysym.sym == SDLK_DOWN) {
                        vy++;
                    } 
                    break;

                    

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h); // Si on redimensionne la fenêtre
                    } else if (event.window.event == SDL_WINDOWEVENT_MOVED) {
                        SDL_GetWindowPosition(window, &x, &y); // Si on bouge la fenêtre
                    }
                    break;
                default:
                    break;
            }
        }

        

        if (!pause) {

            int new_x = x + vx;
            int new_y = y + vy;

            if (new_x <= 0) { // Bord gauche
                new_x = 0;
                vx = -vx;
            }
            else if (new_x + w >= ecran_w) { // Bord droit
                new_x = ecran_w - w;
                vx = -vx;
            }

            if (new_y <= 26) { // Bord haut
                new_y = 0;
                vy = -vy;
            }
            else if (new_y + h >= ecran_h) { // Bord bas
                new_y = ecran_h - h;
                vy = -vy;
            }

            SDL_SetWindowPosition(window, new_x, new_y); // Affiche à la nouvelle position

            x = new_x;
            y = new_y;
            
            printf("Position : (%d, %d), Taille : (%d, %d), Vitesse : (%d, %d)\n", x, y, w, h, vx, vy);

            SDL_Delay(10);
        }
    } 

    SDL_DestroyWindow(window);    //ferme la fenêtre

    SDL_Quit(); //ferme la SDL

    return 0;
}