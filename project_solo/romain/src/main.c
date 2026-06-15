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

    int x, y;
    int w, h;


    while (program_on){                           // Voilà la boucle des évènements 

        if (SDL_PollEvent(&event)){                 // si la file d'évènements n'est pas vide : défiler l'élément en tête
                                                    // de file dans 'event'
            switch(event.type){                       // En fonction de la valeur du type de cet évènement
                case SDL_QUIT :                           // Un évènement simple, on a cliqué sur la x de la fenêtre
                    program_on = SDL_FALSE;                 // Il est temps d'arrêter le programme
                    break;

                default:                                  // L'évènement défilé ne nous intéresse pas
                    break;
            }
        }
        // Affichages et calculs souvent ici
        SDL_GetWindowPosition(window,&x,&y);
        SDL_GetWindowSize(window,&w,&h);

        printf("(%d, %d), (%d, %d)\n", x, y, w, h);
    }  

    SDL_DestroyWindow(window);    //ferme la fenêtre

    SDL_Quit(); //ferme la SDL

    return 0;
}